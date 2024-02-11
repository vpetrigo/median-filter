/**
 * \file
 * \brief
 * \author
 */

#include "optimized_median_filter.h"

#include <stdio.h>
#include <string.h>

void optimized_mf_init(struct optimized_median_filter *filter, struct median_filter_node *buffer, size_t buffer_size)
{
    filter->head.next = &filter->tail;
    filter->tail.next = NULL;
    filter->tail.value = 0;
    filter->median = NULL;
    filter->buffer = buffer;
    filter->size = buffer_size;
    filter->current_pos = 0;
    memset(filter->buffer, 0, filter->size * sizeof(struct median_filter_node));
}

void optimized_mf_insert_value(struct optimized_median_filter *filter, unsigned value)
{
    struct median_filter_node *scan_old = NULL;
    /* Points to pointer to first (largest) datum in chain */
    struct median_filter_node *scan = &filter->head;
    struct median_filter_node *insert = &filter->buffer[filter->current_pos];
    struct median_filter_node *insert_next = insert->next;
    const struct median_filter_node *const end = &filter->tail;

    filter->current_pos = (filter->current_pos + 1) % filter->size;
    insert->value = value;
    /* Median initially to first in chain */
    filter->median = &filter->head;

    /* Handle chain-out of first item in chain as special case */
    if (scan->next == insert) {
        scan->next = insert_next;
    }

    scan_old = scan;
    scan = scan->next;

    /* Loop through the chain, normal loop exit via break. */
    for (size_t i = 0; i < filter->size; ++i) {
        /* Handle odd-numbered item in chain  */
        if (scan->next == insert) {
            scan->next = insert_next; /* Chain out the old datum.*/
        }

        if (scan->value < value) /* If datum is larger than scanned value,*/
        {
            insert->next = scan_old->next; /* Chain it in here.  */
            scan_old->next = insert;       /* Mark it chained in. */
            value = 0;
        };

        /* Step median pointer down chain after doing odd-numbered element */
        filter->median = filter->median->next; /* Step median pointer.  */

        if (scan == end) {
            break;
        }

        scan_old = scan;   /* Save this pointer and   */
        scan = scan->next; /* step down chain */

        /* Handle even-numbered item in chain.  */
        if (scan->next == insert) {
            scan->next = insert_next;
        }

        if (scan->value < value) {
            insert->next = scan_old->next; /* Chain it in here.  */
            scan_old->next = insert;       /* Mark it chained in. */
            value = 0;
        }

        if (scan == end) {
            break;
        }

        scan_old = scan;   /* Save this pointer and   */
        scan = scan->next; /* step down chain */
    }
}

unsigned optimized_mf_get_median(const struct optimized_median_filter *filter)
{
    return filter->median->value;
}

void optimized_mf_debug(const struct optimized_median_filter *filter)
{
    const struct median_filter_node *current = filter->head.next;
    const struct median_filter_node *const end = &filter->tail;

    puts("Print buffer layout:");
    for (size_t i = 0; i < filter->size; ++i) {
        printf("%u ", filter->buffer[i].value);
    }

    puts("\nPrint ordered list:");
    while (current != end) {
        printf("%u", current->value);
        current = current->next;

        if (current != end) {
            printf(" -> ");
        }
    }

    puts("");
}
