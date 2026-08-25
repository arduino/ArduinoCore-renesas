#include "WiFiSSLClient.h"

WiFiSSLClient::WiFiSSLClient()
{
  _connected = false;

  sslclient = new sslclient_context;
  _client = new WiFiClient;

  _timeout = 1000;
  _CA_cert = NULL;
  _CA_path = "/mbedtls";
  _cert = NULL;
  _private_key = NULL;
  _pskIdent = NULL;
  _psKey = NULL;

  ssl_init(sslclient, _client, _CA_path);

  sslclient->handshake_timeout = 5000;
}

WiFiSSLClient::~WiFiSSLClient()
{
  stop();
  delete _client;
}

