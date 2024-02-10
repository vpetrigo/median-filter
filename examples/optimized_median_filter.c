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
    filter->median = NULL;
    filter->buffer = buffer;
    filter->size = buffer_size;
    filter->current_pos = 0;
    memset(filter->buffer, 0, filter->size * sizeof(struct median_filter_node));
}

void optimized_mf_insert_value(struct optimized_median_filter *filter, unsigned value)
{
    struct median_filter_node *scan_old = &filter->head;
    struct median_filter_node *scan = filter->head.next;
    struct median_filter_node *insert = &filter->buffer[filter->current_pos];
    struct median_filter_node *insert_next = insert->next;
    const struct median_filter_node *const end = &filter->tail;

    insert->value = value;
    filter->current_pos = (filter->current_pos + 1) % filter->size;

    if (scan == &filter->tail) {
        insert->next = scan;
        scan_old->next = insert;
    }

    if (scan->next == insert) {
        scan->next = insert_next;
    }

    while (scan != end) {
        if (scan->value < insert->value) {
            scan_old->next = insert;
            insert->next = scan;
            break;
        }

        scan_old = scan;
        scan = scan->next;
    }
}

unsigned optimized_mf_get_median(const struct optimized_median_filter *filter)
{
    const struct median_filter_node *current = filter->head.next;
    const struct median_filter_node *const end = &filter->tail;

    if (current == end) {
        return 0;
    }

    for (size_t i = 0; i < filter->size / 2 && current->next != end; ++i) {
        current = current->next;
    }

    return current->value;
}

void optimized_mf_debug(const struct optimized_median_filter *filter)
{
    const struct median_filter_node *current = filter->head.next;
    const struct median_filter_node *const end = &filter->tail;

    puts("Print buffer layout:");
    for (size_t i = 0; i < filter->size; ++i) {
        printf("%u ", filter->buffer[i].value);
    }

    puts("Print ordered list:");
    while (current != end) {
        printf("%u", current->value);
        current = current->next;

        if (current != end) {
            printf(" -> ");
        }
    }

    puts("");
}
