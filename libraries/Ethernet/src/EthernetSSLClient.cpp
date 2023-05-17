#include "EthernetSSLClient.h"

EthernetSSLClient::EthernetSSLClient()
{
  _connected = false;

  sslclient = new sslclient_context;
  _client = new EthernetClient;
  ssl_init(sslclient, _client);
  _timeout = 1000;
  _CA_cert = NULL;
  _cert = NULL;
  _private_key = NULL;
  _pskIdent = NULL;
  _psKey = NULL;

  sslclient->handshake_timeout = 5000;
}

EthernetSSLClient::~EthernetSSLClient()
{
  stop();
  delete _client;
}

