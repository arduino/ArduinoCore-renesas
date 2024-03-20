#pragma once
#include <lwip/include/lwip/pbuf.h>

/**
 * This function aim to free a pbuf chain that has been partially consumed.
 * @param p the head of the pbuf chain
 * @param copied the size that had been consumed in the last operation
 * @param offset the size that had been consumed in the previous operations,
 *               this value will be updated with the ffset of the new head
 * @return the new pbuf head
 */
struct pbuf* free_pbuf_chain(struct pbuf* p, uint16_t copied, uint16_t *offset);