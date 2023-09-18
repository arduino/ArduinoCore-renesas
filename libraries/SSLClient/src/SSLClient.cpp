/*
  SSLClient.cpp - Base class that provides Client SSL to ESP32
  Additions (c) 2011 Adrian McEwen.  All right reserved.
  Additions Copyright (C) 2017 Evandro Luis Copercini.
  Additions Copyright (C) 2019 Vadim Govorovski.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SSLClient.h"
#include "SE05X.h"
#include "mbedtls/pem.h"
#include <errno.h>

#undef connect
#undef write
#undef read


SSLClient::SSLClient()
{
    _connected = false;

    sslclient = new sslclient_context;

    _timeout = 1000;
    _use_insecure = false;
    _CA_cert = NULL;
    _CA_path = NULL;
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;
}

SSLClient::SSLClient(Client* client)
{
    _connected = false;

    sslclient = new sslclient_context;

    _timeout = 1000;
    _use_insecure = false;
    _CA_cert = NULL;
    _CA_path = "/mbedtls";
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;

    ssl_init(sslclient, client, _CA_path);

    sslclient->handshake_timeout = 5000;
}

SSLClient::SSLClient(Client* client, String ca_path)
{
    _connected = false;

    sslclient = new sslclient_context;

    _timeout = 1000;
    _use_insecure = false;
    _CA_cert = NULL;
    _CA_path = ca_path.c_str();
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;

    ssl_init(sslclient, client, _CA_path);

    sslclient->handshake_timeout = 5000;
}

SSLClient::~SSLClient()
{
    stop();
    delete sslclient;
}

void SSLClient::setClient(Client& client)
{
    ssl_init(sslclient, &client, _CA_path);

    sslclient->handshake_timeout = 5000;
}

void SSLClient::stop()
{
    if (sslclient->client >= 0) {
        //sslclient->client->stop();
        _connected = false;
        _peek = -1;
    }
    stop_ssl_socket(sslclient, _CA_cert, _cert, _private_key);
}

int SSLClient::connect(IPAddress ip, uint16_t port)
{
    if (_pskIdent && _psKey)
        return connect(ip, port, _pskIdent, _psKey);
    return connect(ip, port, _CA_cert, _CA_path, _cert, _private_key);
}

int SSLClient::connect(IPAddress ip, uint16_t port, int32_t timeout){
    _timeout = timeout;
    return connect(ip, port);
}

int SSLClient::connect(const char *host, uint16_t port)
{
    if (_pskIdent && _psKey)
        return connect(host, port, _pskIdent, _psKey);
    return connect(host, port, _CA_cert, _CA_path, _cert, _private_key);
}

int SSLClient::connect(const char *host, uint16_t port, int32_t timeout){
    _timeout = timeout;
    return connect(host, port);
}

int SSLClient::connect(IPAddress ip, uint16_t port, const char *_CA_cert, const char *_CA_path, const char *_cert, const char *_private_key)
{
    return connect(ip.toString().c_str(), port, _CA_cert, _CA_path, _cert, _private_key);
}

int SSLClient::connect(const char *host, uint16_t port, const char *_CA_cert, const char *_CA_path, const char *_cert, const char *_private_key)
{
    log_d("Connecting to %s:%d", host, port);
    int ret = start_ssl_client(sslclient, host, port, _timeout, _CA_cert, _CA_path, _cert, _private_key, NULL, NULL, _use_insecure);
    _lastError = ret;
    if (ret < 0) {
        log_e("start_ssl_client: %d", ret);
        stop();
        return 0;
    }
    log_i("SSL connection established");
    _connected = true;
    return 1;
}

int SSLClient::connect(IPAddress ip, uint16_t port, const char *pskIdent, const char *psKey) {
    return connect(ip.toString().c_str(), port,_pskIdent, _psKey);
}

int SSLClient::connect(const char *host, uint16_t port, const char *pskIdent, const char *psKey) {
    log_v("start_ssl_client with PSK");
    int ret = start_ssl_client(sslclient, host, port, _timeout, NULL, NULL, NULL, NULL, _pskIdent, _psKey, _use_insecure);
    _lastError = ret;
    if (ret < 0) {
        log_e("start_ssl_client: %d", ret);
        stop();
        return 0;
    }
    _connected = true;
    return 1;
}

int SSLClient::peek(){
    if(_peek >= 0){
        return _peek;
    }
    _peek = timedRead();
    return _peek;
}

size_t SSLClient::write(uint8_t data)
{
    return write(&data, 1);
}

int SSLClient::read()
{
    uint8_t data = -1;
    int res = read(&data, 1);
    if (res < 0) {
        return res;
    }
    return data;
}

size_t SSLClient::write(const uint8_t *buf, size_t size)
{
    if (!_connected) {
        return 0;
    }
    int res = send_ssl_data(sslclient, buf, size);
    if (res < 0) {
        stop();
        res = 0;
    }
    return res;
}

int SSLClient::read(uint8_t *buf, size_t size)
{
    int peeked = 0;
    int avail = available();
    if ((!buf && size) || avail <= 0) {
        return -1;
    }
    if(!size){
        return 0;
    }
    if(_peek >= 0){
        buf[0] = _peek;
        _peek = -1;
        size--;
        avail--;
        if(!size || !avail){
            return 1;
        }
        buf++;
        peeked = 1;
    }
    
    int res = get_ssl_receive(sslclient, buf, size);
    if (res < 0) {
        stop();
        return peeked?peeked:res;
    }
    return res + peeked;
}

int SSLClient::available()
{
    int peeked = (_peek >= 0);
    if (!_connected) {
        return peeked;
    }
    int res = data_to_read(sslclient);
    if (res < 0) {
        stop();
        return peeked?peeked:res;
    }
    return res+peeked;
}

uint8_t SSLClient::connected()
{
    uint8_t dummy = 0;
    read(&dummy, 0);

    return _connected;
}

void SSLClient::setInsecure()
{
    _CA_cert = NULL;
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;
    _use_insecure = true;
}

void SSLClient::setCACert (const char *rootCA)
{
    log_d("Set root CA");
    _CA_cert = rootCA;
}

void SSLClient::setCAPath (const char *rootCAPath)
{
    log_d("Set root CA Path");
    _CA_path = rootCAPath;
}

void SSLClient::setCertificate (const char *client_ca)
{
    log_d("Set client CA");
    _cert = client_ca;
}

void SSLClient::setPrivateKey (const char *private_key)
{
    log_d("Set client PK");
    _private_key = private_key;
}

void SSLClient::setPreSharedKey(const char *pskIdent, const char *psKey) {
    log_d("Set PSK");
    _pskIdent = pskIdent;
    _psKey = psKey;
}

bool SSLClient::verify(const char* fp, const char* domain_name)
{
    if (!sslclient)
        return false;

    return verify_ssl_fingerprint(sslclient, fp, domain_name);
}

char *SSLClient::_streamLoad(Stream& stream, size_t size) {
  char *dest = (char*)malloc(size+1);
  if (!dest) {
    return nullptr;
  }
  if (size != stream.readBytes(dest, size)) {
    free(dest);
    dest = nullptr;
    return nullptr;
  }
  dest[size] = '\0';
  return dest;
}

bool SSLClient::loadCACert(Stream& stream, size_t size) {
  if (_CA_cert != NULL) free(const_cast<char*>(_CA_cert));
  char *dest = _streamLoad(stream, size);
  bool ret = false;
  if (dest) {
    setCACert(dest);
    ret = true;
  }
  return ret;
}

bool SSLClient::loadCertificate(Stream& stream, size_t size) {
  if (_cert != NULL) free(const_cast<char*>(_cert));
  char *dest = _streamLoad(stream, size);
  bool ret = false;
  if (dest) {
    setCertificate(dest);
    ret = true;
  }
  return ret;
}

bool SSLClient::loadPrivateKey(Stream& stream, size_t size) {
  if (_private_key != NULL) free(const_cast<char*>(_private_key));
  char *dest = _streamLoad(stream, size);
  bool ret = false;
  if (dest) {
    setPrivateKey(dest);
    ret = true;
  }
  return ret;
}

void SSLClient::setEccSlot(int KeySlot, const byte cert[], int certLen) {
    unsigned char buf[1024];
    size_t olen;
    int ret;

    if ((ret = mbedtls_pem_write_buffer("-----BEGIN CERTIFICATE-----\n",
                                        "-----END CERTIFICATE-----\n",
                                        cert, certLen,
                                        &buf[0], sizeof(buf), &olen)) != 0) {
        log_e("setEccSlot: client certificate error");
    }

    char *dest = (char*)malloc(olen+1);
    memcpy(dest,buf,olen);
    setCertificate(dest);
    log_d("%s", _cert);

    uint8_t key[121] = {
        0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0xA5, 0xA6, 0xB5,
        0xB6, 0xA5, 0xA6, 0xB5, 0xB6, 0x10, 0x00, 0xA0, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
        0x03, 0x01, 0x07, 0xA1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xC1, 0x84, 0x04, 0x32, 0x06, 0x0A, 0x8C,
        0x0C, 0xDB, 0xB4, 0x61, 0x40, 0x98, 0xD4, 0xF9, 0x41, 0xB4, 0x1F, 0x99, 0x33, 0x6F, 0xCF, 0xE0,
        0x53, 0x1B, 0xAF, 0xD5, 0xF9, 0x8B, 0xEC, 0x27, 0x90, 0x40, 0xA9, 0xE0, 0x86, 0xFC, 0x27, 0xBD,
        0xD8, 0x20, 0x53, 0x01, 0x7B, 0xF6, 0xBC, 0x59, 0x0F, 0xE4, 0x93, 0xE6, 0xE3, 0x05, 0xBB, 0xBE,
        0x83, 0xA3, 0x5E, 0x5B, 0x64, 0x1D, 0x29, 0xED, 0x85
    };

    key[28] = KeySlot;

    if ((ret = mbedtls_pem_write_buffer("-----BEGIN EC PRIVATE KEY-----\n",
                                        "-----END EC PRIVATE KEY-----\n",
                                        (const unsigned char *)key, 121,
                                        &buf[0], sizeof(buf), &olen)) != 0) {
        log_e("setEccSlot: client key error");
    }

    dest = (char*)malloc(olen+1);
    memcpy(dest,buf,olen);
    setPrivateKey(dest);
    log_d("%s", _private_key);
}

int SSLClient::lastError(char *buf, const size_t size)
{
    if (!_lastError) {
        return 0;
    }
    mbedtls_strerror(_lastError, buf, size);
    return _lastError;
}

void SSLClient::setHandshakeTimeout(unsigned long handshake_timeout)
{
    sslclient->handshake_timeout = handshake_timeout * 1000;
}

void SSLClient::setConnectionTimeout(unsigned long seconds)
{
    _timeout = seconds * 1000;
}
