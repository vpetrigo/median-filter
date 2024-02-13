/**
 * \file
 * \brief
 * \author Vladimir Petrigo
 */

#include "optimized_median_filter.h"

#include <stdio.h>
#include <string.h>

// PRIVATE FUNCTION DECLARATIONS

static inline size_t get_insert_position(const struct optimized_median_filter *filter)
{
    return filter->current_pos;
}

static inline size_t get_next_position(const struct optimized_median_filter *filter)
{
    return (filter->current_pos + 1) % filter->size;
}

static inline void find_median(struct optimized_median_filter *filter)
{
    struct median_filter_node *it = filter->smallest;

    for (size_t i = 0; i < filter->size / 2 && it->next != NULL; ++i) {
        it = it->next;
    }

    filter->median = it;
}

static inline void remove_node_from_list(struct median_filter_node *node)
{
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
}

static inline void insert_after_node(struct median_filter_node *node, struct median_filter_node *insert)
{
    if (node->next != NULL) {
        node->next->prev = insert;
        insert->next = node->next;
    }

    node->next = insert;
    insert->prev = node;
}

static inline void insert_before_node(struct median_filter_node *node, struct median_filter_node *insert)
{
    if (node->prev != NULL) {
        node->prev->next = insert;
        insert->prev = node->prev;
    }

    insert->next = node;
    node->prev = insert;
}

void optimized_mf_init(struct optimized_median_filter *filter, struct median_filter_node *buffer, size_t buffer_size)
{
    filter->smallest = NULL;
    filter->median = NULL;
    filter->buffer = buffer;
    filter->size = buffer_size;
    filter->current_pos = 0;
    memset(filter->buffer, 0, filter->size * sizeof(struct median_filter_node));
}

void optimized_mf_insert_value(struct optimized_median_filter *filter, unsigned value)
{
    const size_t insert_pos = get_insert_position(filter);
    struct median_filter_node *insert = &filter->buffer[insert_pos];

    if (filter->smallest == NULL) {
        filter->smallest = insert;
        insert->value = value;
        filter->median = filter->smallest;
        goto exit;
    }

    if (filter->smallest == insert) {
        filter->smallest = insert->next;
    }

    insert->value = value;
    remove_node_from_list(insert);

    struct median_filter_node *current = filter->smallest;

    while (current->next != NULL) {
        if (current->value > insert->value) {
            break;
        }

        current = current->next;
    }

    if (current->value > insert->value) {
        if (current == filter->smallest) {
            filter->smallest = insert;
        }

        insert_before_node(current, insert);
    }
    else {
        insert_after_node(current, insert);
    }

exit:
    filter->current_pos = get_next_position(filter);
    find_median(filter);
}

unsigned optimized_mf_get_median(const struct optimized_median_filter *filter)
{
    return filter->median->value;
}

void optimized_mf_debug(const struct optimized_median_filter *filter)
{
    const struct median_filter_node *current = filter->smallest;

    printf("Tail value: %u\n", filter->smallest->value);
    puts("Print buffer layout:");

    for (size_t i = 0; i < filter->size; ++i) {
        printf("%u ", filter->buffer[i].value);
    }

    puts("\nPrint ordered list:");

    while (current != NULL) {
        printf("%u", current->value);
        current = current->next;

        if (current != NULL) {
            printf(" -> ");
        }
    }

    puts("");
}
