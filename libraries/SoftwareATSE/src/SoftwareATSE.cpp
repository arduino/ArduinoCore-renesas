/*
  SoftwareATSE.cpp
  Copyright (c) 2023 Arduino SA.  All right reserved.

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

#include "SoftwareATSE.h"
#include "Modem.h"

using namespace std;

static uint8_t constexpr SATSE_SHA256_LENGTH = 32;
static uint8_t constexpr SATSE_EC256_SIGNATURE_LENGTH = 64;
static uint8_t constexpr SATSE_EC256_PUB_KEY_LENGTH = 64;

SoftwareATSEClass::SoftwareATSEClass() {

}

SoftwareATSEClass::~SoftwareATSEClass() {
    end();
}

int SoftwareATSEClass::begin(const char * name, bool readOnly, const char* partition_label)
{
    string res = "";
    modem.begin();
    if (name != nullptr && strlen(name) > 0) {
        if (modem.write(string(PROMPT(_SOFTSE_BEGIN)), res, "%s%s,%d,%s\r\n", CMD_WRITE(_SOFTSE_BEGIN), name, readOnly, partition_label != NULL ? partition_label : "")) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

void SoftwareATSEClass::end()
{
    string res = "";
    modem.write(string(PROMPT(_SOFTSE_END)), res, "%s", CMD(_SOFTSE_END));
}

void SoftwareATSEClass::debug(Stream  &u, uint8_t level)
{
    modem.debug(u,level);
}

int SoftwareATSEClass::serialNumber(byte sn[])
{
    string res = "";
    modem.read_using_size();
    if (modem.write(string(PROMPT(_SOFTSE_SERIAL)), res, "%s", CMD(_SOFTSE_SERIAL))) {
        if (res.size()) {
            String result = (char*)NULL;
            memcpy(sn, (uint8_t*)&res[0], 6);
            return 1;
        }
    }
    return 0;
}

String SoftwareATSEClass::serialNumber()
{
    string res = "";
    modem.read_using_size();
    if (modem.write(string(PROMPT(_SOFTSE_SERIAL)), res, "%s", CMD(_SOFTSE_SERIAL))) {
        if (res.size()) {
            String result = (char*)NULL;
            result.reserve(res.size() * 2);

            for (size_t i = 0; i < res.size(); i++) {
                byte b = res.data()[i];

                if (b < 16) {
                    result += "0";
                }
                result += String(b, HEX);
            }
            result.toUpperCase();
            return result;
        }
    }
    return "";
}

long SoftwareATSEClass::random(long max)
{
    return random(0, max);
}

long SoftwareATSEClass::random(long min, long max)
{
    if (min >= max)
    {
        return min;
    }

    long diff = max - min;

    long r;
    random((byte*)&r, sizeof(r));

    if (r < 0) {
        r = -r;
    }

    r = (r % diff);

    return (r + min);
}

int SoftwareATSEClass::random(byte data[], size_t length)
{
    string res = "";
    if (data != nullptr && length > 0) {
        modem.read_using_size();
        if (modem.write(string(PROMPT(_SOFTSE_RND)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_RND), length)) {
            if (res.size() >= length) {
                memcpy(data, (uint8_t*)&res[0], length);
                return 1;
            }
        }
    }
    return 0;
}

int SoftwareATSEClass::generatePrivateKey(int keyID, byte publicKey[])
{
    string res = "";
    if (publicKey != nullptr) {
        modem.read_using_size();
        if (modem.write(string(PROMPT(_SOFTSE_PRI_KEY)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_PRI_KEY), keyID)) {
            if (res.size() == SATSE_EC256_PUB_KEY_LENGTH) {
                memcpy(publicKey, (uint8_t*)&res[0], SATSE_EC256_PUB_KEY_LENGTH);
                return 1;
            }
        }
    }
    return 0;
}

int SoftwareATSEClass::generatePublicKey(int keyID, byte publicKey[])
{
    string res = "";
    if (publicKey != nullptr) {
        modem.read_using_size();
        if (modem.write(string(PROMPT(_SOFTSE_PUB_KEY)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_PUB_KEY), keyID)) {
            if (res.size() == SATSE_EC256_PUB_KEY_LENGTH) {
                memcpy(publicKey, (uint8_t*)&res[0], SATSE_EC256_PUB_KEY_LENGTH);
                return 1;
            }
        }
    }
    return 0;
}

int SoftwareATSEClass::ecSign(int slot, const byte message[], byte signature[])
{
    string res = "";
    if ( message != nullptr) {
        modem.write_nowait(string(PROMPT(_SOFTSE_S_V_BUF_SET)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_S_V_BUF_SET), SATSE_SHA256_LENGTH);
        if(!modem.passthrough(message, SATSE_SHA256_LENGTH)) {
            return 0;
        }
    }
    
    if (signature != nullptr) {
        modem.read_using_size();
        if (modem.write(string(PROMPT(_SOFTSE_SIGN_GET)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_SIGN_GET), slot)) {
            if (res.size() == SATSE_EC256_SIGNATURE_LENGTH) {
                memcpy(signature, (uint8_t*)&res[0], SATSE_EC256_SIGNATURE_LENGTH);
                return 1;
            }
        }
    }
    return 0;
}

int SoftwareATSEClass::ecdsaVerify(const byte message[], const byte signature[], const byte pubkey[])
{
    string res = "";
    if ( message != nullptr && signature!= nullptr) {
        static const byte len = SATSE_SHA256_LENGTH + SATSE_EC256_SIGNATURE_LENGTH + SATSE_EC256_PUB_KEY_LENGTH;
        byte tmp[len];
        memcpy(tmp, message, SATSE_SHA256_LENGTH);
        memcpy(&tmp[SATSE_SHA256_LENGTH], signature, SATSE_EC256_SIGNATURE_LENGTH);
        memcpy(&tmp[SATSE_SHA256_LENGTH + SATSE_EC256_SIGNATURE_LENGTH], pubkey, SATSE_EC256_PUB_KEY_LENGTH);
        modem.write_nowait(string(PROMPT(_SOFTSE_S_V_BUF_SET)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_S_V_BUF_SET), len);
        if(!modem.passthrough(tmp, len)) {
            return 0;
        }
    }
    
    if (signature != nullptr) {
        if (modem.write(string(PROMPT(_SOFTSE_VERIFY_GET)), res, "%s", CMD(_SOFTSE_VERIFY_GET))) {
            return atoi(res.c_str()) == 0 ? 1: 0;
        }
    }
    return 0;
}

int SoftwareATSEClass::SHA256(const uint8_t *buffer, size_t size, uint8_t *digest) {
    string res = "";
    if ( buffer != nullptr) {
        modem.write_nowait(string(PROMPT(_SOFTSE_S_V_BUF_SET)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_S_V_BUF_SET), size);
        if(!modem.passthrough(buffer, size)) {
            return 0;
        }
    }

    if (digest != nullptr) {
        modem.read_using_size();
        if (modem.write(string(PROMPT(_SOFTSE_SHA256_GET)), res, "%s", CMD(_SOFTSE_SHA256_GET))) {
            if (res.size() == SATSE_SHA256_LENGTH) {
                memcpy(digest, (uint8_t*)&res[0], SATSE_SHA256_LENGTH);
                return 1;
            }
        }
    }
    return 0;
}

int SoftwareATSEClass::readSlot(int slot, byte data[], int length)
{
    string res = "";
    if (data != nullptr) {
        modem.read_using_size();
        if (modem.write(string(PROMPT(_SOFTSE_READ_SLOT)), res, "%s%d\r\n", CMD_WRITE(_SOFTSE_READ_SLOT), slot)) {
            if (res.size()) {
                int len = res.size() > length ? length : res.size();
                memcpy(data, (uint8_t*)&res[0], len);
                return 1;
            }
        }
    }
    return 0;
}

int SoftwareATSEClass::writeSlot(int slot, const byte data[], int length)
{
    string res = "";
    if ( data != nullptr && length > 0) {
        modem.write_nowait(string(PROMPT(_SOFTSE_WRITE_SLOT)), res, "%s%d,%d\r\n", CMD_WRITE(_SOFTSE_WRITE_SLOT), slot, length);
        if(modem.passthrough(data, length)) {
            return 1;
        }
    }
    return 0;
}

SoftwareATSEClass SATSE;
