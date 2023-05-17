#include "lwipTcp.h"

#if LWIP_TCP
static err_t tcp_recv_callback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err);
static err_t tcp_sent_callback(void* arg, struct tcp_pcb* tpcb, u16_t len);
static void tcp_err_callback(void* arg, err_t err);
/**
 * @brief Function called when TCP connection established
 * @param arg: user supplied argument
 * @param tpcb: pointer on the connection control block
 * @param err: when connection correctly established err should be ERR_OK
 * @retval err_t: returned error
 */
err_t tcp_connected_callback(void* arg, struct tcp_pcb* tpcb, err_t err)
{
    struct tcp_struct* tcp_arg = (struct tcp_struct*)arg;

    if (err == ERR_OK) {
        if ((tcp_arg != NULL) && (tcp_arg->pcb == tpcb)) {
            tcp_arg->state = TCP_CONNECTED;

            /* initialize LwIP tcp_recv callback function */
            tcp_recv(tpcb, tcp_recv_callback);

            /* initialize LwIP tcp_sent callback function */
            tcp_sent(tpcb, tcp_sent_callback);

            /* initialize LwIP tcp_err callback function */
            tcp_err(tpcb, tcp_err_callback);

            return ERR_OK;
        } else {
            /* close connection */
            tcp_connection_close(tpcb, tcp_arg);

            return ERR_ARG;
        }
    } else {
        /* close connection */
        tcp_connection_close(tpcb, tcp_arg);
    }
    return err;
}

/**
 * @brief  This function is the implementation of tcp_accept LwIP callback
 * @param arg user supplied argument
 * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
 * @param err: when connection correctly established err should be ERR_OK
 * @retval err_t: error status
 */
err_t tcp_accept_callback(void* arg, struct tcp_pcb* newpcb, err_t err)
{
    err_t ret_err;
    uint8_t accepted;
    struct tcp_struct** tcpClient = (struct tcp_struct**)arg;

    /* set priority for the newly accepted tcp connection newpcb */
    tcp_setprio(newpcb, TCP_PRIO_MIN);

    if ((tcpClient != NULL) && (ERR_OK == err)) {
        struct tcp_struct* client = (struct tcp_struct*)mem_malloc(sizeof(struct tcp_struct));

        if (client != NULL) {
            client->state = TCP_ACCEPTED;
            client->pcb = newpcb;
            client->data.p = NULL;
            client->data.available = 0;

            /* Looking for an empty socket */
            for (uint16_t i = 0; i < MAX_CLIENT; i++) {
                if (tcpClient[i] == NULL) {
                    tcpClient[i] = client;
                    accepted = 1;
                    break;
                }
            }

            if (accepted) {
                /* pass newly allocated client structure as argument to newpcb */
                tcp_arg(newpcb, client);

                /* initialize lwip tcp_recv callback function for newpcb  */
                tcp_recv(newpcb, tcp_recv_callback);

                /* initialize lwip tcp_err callback function for newpcb  */
                tcp_err(newpcb, tcp_err_callback);

                /* initialize LwIP tcp_sent callback function */
                tcp_sent(newpcb, tcp_sent_callback);

                ret_err = ERR_OK;
            } else {
                /*  close tcp connection */
                tcp_connection_close(newpcb, client);
                mem_free(client);

                /* return memory error */
                ret_err = ERR_MEM;
            }
        } else {
            /*  close tcp connection */
            tcp_connection_close(newpcb, client);
            mem_free(client);

            /* return memory error */
            ret_err = ERR_MEM;
        }
    } else {
        tcp_close(newpcb);
        ret_err = ERR_ARG;
    }
    return ret_err;
}

/**
 * @brief tcp_receiv callback
 * @param arg: argument to be passed to receive callback
 * @param tpcb: tcp connection control block
 * @param err: receive error code
 * @retval err_t: returned error
 */
static err_t tcp_recv_callback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err)
{
    struct tcp_struct* tcp_arg = (struct tcp_struct*)arg;
    err_t ret_err;

    /* if we receive an empty tcp frame from server => close connection */
    if (p == NULL) {
        /* we're done sending, close connection */
        tcp_connection_close(tpcb, tcp_arg);
        ret_err = ERR_OK;
    }
    /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
    else if (err != ERR_OK) {
        /* free received pbuf*/
        if (p != NULL) {
            pbuf_free(p);
        }
        ret_err = err;
    } else if ((tcp_arg->state == TCP_CONNECTED) || (tcp_arg->state == TCP_ACCEPTED)) {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

        if (tcp_arg->data.p == NULL) {
            tcp_arg->data.p = p;
        } else {
            pbuf_chain(tcp_arg->data.p, p);
        }

        tcp_arg->data.available += p->len;
        ret_err = ERR_OK;
    }
    /* data received when connection already closed */
    else {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

        /* free pbuf and do nothing */
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/**
 * @brief  This function implements the tcp_sent LwIP callback (called when ACK
 *         is received from remote host for sent data)
 * @param  arg: pointer on argument passed to callback
 * @param  tcp_pcb: tcp connection control block
 * @param  len: length of data sent
 * @retval err_t: returned error code
 */
static err_t tcp_sent_callback(void* arg, struct tcp_pcb* tpcb, u16_t len)
{
    struct tcp_struct* tcp_arg = (struct tcp_struct*)arg;

    LWIP_UNUSED_ARG(len);

    if ((tcp_arg != NULL) && (tcp_arg->pcb == tpcb)) {
        return ERR_OK;
    }

    return ERR_ARG;
}

/** Function prototype for tcp error callback functions. Called when the pcb
 * receives a RST or is unexpectedly closed for any other reason.
 *
 * @note The corresponding pcb is already freed when this callback is called!
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param err Error code to indicate why the pcb has been closed
 *            ERR_ABRT: aborted through tcp_abort or by a TCP timer
 *            ERR_RST: the connection was reset by the remote host
 */
static void tcp_err_callback(void* arg, err_t err)
{
    struct tcp_struct* tcp_arg = (struct tcp_struct*)arg;

    if (tcp_arg != NULL) {
        if (ERR_OK != err) {
            tcp_arg->pcb = NULL;
            tcp_arg->state = TCP_CLOSING;
        }
    }
}

/**
 * @brief This function is used to close the tcp connection with server
 * @param tpcb: tcp connection control block
 * @param es: pointer on echoclient structure
 * @retval None
 */
void tcp_connection_close(struct tcp_pcb* tpcb, struct tcp_struct* tcp)
{
    /* remove callbacks */
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    tcp_err(tpcb, NULL);
    tcp_accept(tpcb, NULL);

    /* close tcp connection */
    tcp_close(tpcb);

    tcp->pcb = NULL;
    tcp->state = TCP_CLOSING;
}

#endif /* LWIP_TCP */
