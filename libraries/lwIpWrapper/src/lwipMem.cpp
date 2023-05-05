#include "lwipMem.h"

/* -------------------------------------------------------------------------- */
/*                          MEMORY ALLOCATION FUNCTIONS                       */
/* -------------------------------------------------------------------------- */
/**
 * @brief  Allocate a pbuf with data pass in parameter
 * @param  p: pointer to pbuf
 * @param  buffer: pointer to data to store
 * @param  size: number of data to store
 * @retval pointer to the pbuf allocated
 */
struct pbuf* pbuffer_put_data(struct pbuf* p, const uint8_t* buffer, size_t size)
{
    // Allocate memory if pbuf doesn't exit yet.
    if (p == NULL) {
        p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);

        if (p != NULL) {
            // Copy data inside pbuf
            if (ERR_OK == pbuf_take(p, (uint8_t*)buffer, size)) {
                return p;
            } else {
                pbuf_free(p);
            }
        }
    }
    // If pbuf allocated, grow the size of pbuf and add new data
    // NOTE: pbuf_realloc can't be used to grow the size of pbuf
    else {
        struct pbuf* q = pbuf_alloc(PBUF_TRANSPORT, size + p->tot_len, PBUF_RAM);

        if (q != NULL) {
            if (ERR_OK == pbuf_copy(q, p)) {
                if (ERR_OK == pbuf_take_at(q, (uint8_t*)buffer, size, p->tot_len)) {
                    pbuf_free(p);
                    p = q;
                    return p;
                }
            }

            pbuf_free(q);
        }
    }

    return 0;
}

/**
 * @brief  Free pbuf
 *
 * @param  p: pointer to pbuf
 * @retval return always NULL
 */
struct pbuf* pbuffer_free_data(struct pbuf* p)
{
    uint16_t n;

    if (p != NULL) {
        do {
            n = pbuf_free(p);
        } while (n == 0);
    }

    return NULL;
}

/**
 * @brief This function passes pbuf data to uin8_t buffer. It takes account if
 * pbuf is chained.
 * @param data pointer to data structure
 * @param buffer the buffer where write the data read
 * @param size the number of data to read
 * @retval number of data read
 */
uint16_t pbuffer_get_data(struct pbuf_data* data, uint8_t* buffer, size_t size)
{
    uint16_t i;
    uint16_t offset;
    uint16_t nb;
    struct pbuf* ptr;

    if ((data->p == NULL) || (buffer == NULL) || (size == 0) || (data->available == 0) || (data->available > data->p->tot_len)) {
        return 0;
    }

    nb = 0;

    while ((nb < size) && (data->p != NULL) && (data->available > 0)) {
        ptr = data->p;
        offset = ptr->tot_len - data->available;

        /* Get data from p */
        for (i = 0; (nb < size) && ((offset + i) < ptr->len) && (data->available > 0); i++) {
            buffer[nb] = pbuf_get_at(ptr, offset + i);
            nb++;
            data->available--;
        }

        if (nb < size) {
            /* continue with next pbuf in chain (if any) */
            data->p = ptr->next;

            if (data->p != NULL) {
                /* increment reference count for p */
                pbuf_ref(data->p);
            }

            /* chop first pbuf from chain */
            ptr = pbuffer_free_data(ptr);
        }
    }

    if (data->available == 0) {
        data->p = pbuffer_free_data(data->p);
    }

    return nb;
}