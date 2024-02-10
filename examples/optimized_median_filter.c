/**
 * \file
 * \brief
 * \author
 */

#include "optimized_median_filter.h"

#include <stdio.h>
#include <string.h>

void optimized_mf_init(struct optimized_median_filter *filter, struct median_filter_node *main_buffer,
                       size_t buffer_size)
{
    filter->head = NULL;
    filter->tail = NULL;
    filter->buffer = main_buffer;
    filter->current_pos = 0;
    filter->size = buffer_size;
    memset(filter->buffer, 0, filter->size * sizeof(struct median_filter_node));
}

void optimized_mf_insert_value(struct optimized_median_filter *filter, unsigned value)
{
    if (filter->head == NULL) {
        filter->head = &filter->buffer[filter->current_pos];
        filter->tail = filter->head;
    }

    struct median_filter_node *new = filter->tail;
    struct median_filter_node *current = filter->head;
    struct median_filter_node *next = filter->head->next;

    new->value = value;
    filter->current_pos = (filter->current_pos + 1) % filter->size;
    filter->tail = &filter->buffer[filter->current_pos];

    while (current != filter->tail) {
        if (current->value < new->value) {
            filter->head = new;
            new->next = current;
            break;
        }

        current = current->next;
    }

    if (next != NULL) {
        next->next = filter->tail;
    }
}

unsigned optimized_mf_get_median(const struct optimized_median_filter *filter)
{
    if (filter->head == NULL) {
        return 0;
    }

    const struct median_filter_node *it = filter->head;
    size_t counter = 0;
    size_t median_index = filter->size / 2;

    if (filter->size % 2 == 1) {
        --median_index;
    }

    while (it->next != NULL && it->next != filter->tail && counter < filter->size / 2) {
        it = it->next;
    }

    return it->value;
}

void optimized_mf_debug(const struct optimized_median_filter *filter)
{
    const struct median_filter_node *current = filter->head;
    const struct median_filter_node *const end = filter->tail;

    if (current == NULL) {
        return;
    }

    if (current->next == end) {
        printf("%u\n", current->value);
        return;
    }

    while (current != end) {
        printf(" %u ->", current->value);
        current = current->next;
    }

    puts("");
}
