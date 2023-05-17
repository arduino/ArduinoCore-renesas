/* Provide SSL/TLS functions to ESP32 with Arduino
 * by Evandro Copercini - 2017 - Apache 2.0 License
 * Additions Copyright (C) 2019 Vadim Govorovski.
 */

#ifndef ARD_SSL_H
#define ARD_SSL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

#ifdef __cplusplus
}
#endif

#include "ssl_debug.h"
#include <Client.h>

typedef struct sslclient_context {
    Client* client;

    mbedtls_ssl_context ssl_ctx;
    mbedtls_ssl_config ssl_conf;

    mbedtls_ctr_drbg_context drbg_ctx;
    mbedtls_entropy_context entropy_ctx;

    mbedtls_x509_crt ca_cert;
    mbedtls_x509_crt client_cert;
    mbedtls_pk_context client_key;

    char* ca_path;

    unsigned long handshake_timeout;
} sslclient_context;


void ssl_init(sslclient_context *ssl_client, Client *client, const char *ca_path);
int start_ssl_client(sslclient_context *ssl_client, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *rootCAPath, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey, bool insecure);
void stop_ssl_socket(sslclient_context *ssl_client, const char *rootCABuff, const char *cli_cert, const char *cli_key);
int data_to_read(sslclient_context *ssl_client);
int send_ssl_data(sslclient_context *ssl_client, const uint8_t *data, uint16_t len);
int get_ssl_receive(sslclient_context *ssl_client, uint8_t *data, int length);
bool verify_ssl_fingerprint(sslclient_context *ssl_client, const char* fp, const char* domain_name);
bool verify_ssl_dn(sslclient_context *ssl_client, const char* domain_name);

#endif
