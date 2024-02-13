/**
 * \file
 * \brief
 * \author
 */

#ifndef MEDIAN_FILTER_OPTIMIZED_MEDIAN_FILTER_H
#define MEDIAN_FILTER_OPTIMIZED_MEDIAN_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

struct median_filter_node {
    struct median_filter_node *prev;
    struct median_filter_node *next;
    unsigned value;
};

struct optimized_median_filter {
    struct median_filter_node *buffer;
    struct median_filter_node *smallest;
    struct median_filter_node *median;
    size_t current_pos;
    size_t size;
};

void optimized_mf_init(struct optimized_median_filter *filter, struct median_filter_node *buffer, size_t buffer_size);
void optimized_mf_insert_value(struct optimized_median_filter *filter, unsigned value);
unsigned optimized_mf_get_median(const struct optimized_median_filter *filter);

void optimized_mf_debug(const struct optimized_median_filter *filter);

#ifdef __cplusplus
}
#endif

#endif /* MEDIAN_FILTER_OPTIMIZED_MEDIAN_FILTER_H */
