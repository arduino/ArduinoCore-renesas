#include "lwippbuf.h"
#include "utils.h"

struct pbuf* free_pbuf_chain(struct pbuf* p, uint16_t copied, uint16_t *offset) {
    arduino::lock();
    /*
     * free pbufs that have been copied, if copied == 0 we have an error
     * free the buffer chain starting from the head up to the last entire pbuf ingested
     * taking into account the previously not entirely consumed pbuf
     */
    uint32_t tobefreed = 0;
    copied += *offset;

    // in order to clean up the chain we need to find the pbuf in the last pbuf in the chain
    // that got completely consumed by the application, dechain it from it successor and delete the chain before it

    struct pbuf *head = p, *last=head, *prev=nullptr; // FIXME little optimization prev can be substituted by last->next

    while(last!=nullptr && last->len + tobefreed <= copied) {
        tobefreed += last->len;
        prev = last;
        last = last->next;
    }

    // dechain if we are not at the end of the chain (last == nullptr)
    // and if we haven't copied entirely the first pbuf (prev == nullptr) (head == last)
    // if we reached the end of the chain set the this pbuf pointer to nullptr
    if(prev != nullptr) {
        prev->next = nullptr;
        p = last;
    }

    // the chain that is referenced by head is detached by the one referenced by p
    // free the chain if we haven't copied entirely the first pbuf (prev == nullptr)
    if(p != head) {
        uint8_t refs = pbuf_free(head);
    }

    *offset = copied - tobefreed; // This offset should be referenced to the first pbuf in queue

    arduino::unlock();

    return p;
}