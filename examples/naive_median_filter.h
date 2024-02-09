/**
 * \file
 * \brief
 * \author
 */

#ifndef MEDIAN_FILTER_NAIVE_MEDIAN_FILTER_H
#define MEDIAN_FILTER_NAIVE_MEDIAN_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

struct median_filter {
    unsigned *buffer;
    unsigned *sorted_buffer;
    size_t buffer_size;
    size_t current_pos;
    size_t size;
};

void mf_init(struct median_filter *filter, unsigned *main_buffer, unsigned *helper_buffer, size_t buffer_size);
void mf_insert_value(struct median_filter *filter, unsigned value);
unsigned mf_get_median(const struct median_filter *filter);

void mf_debug(const struct median_filter *filter);

#ifdef __cplusplus
}
#endif

#endif /* MEDIAN_FILTER_NAIVE_MEDIAN_FILTER_H */
