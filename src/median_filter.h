/**
 * \brief
 * \author Vladimir Petrigo
 */
#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

struct median_filter {
    unsigned *buffer;
    size_t buffer_size;
    size_t current_pos;
};

bool median_filter_init(struct median_filter *filter, unsigned *buffer, size_t buffer_size);
bool median_filter_insert_number(struct median_filter *filter, unsigned sample);
unsigned median_filter_get_median(const struct median_filter *filter);

#ifdef __cplusplus
}
#endif

#endif /* MEDIAN_FILTER_H */
