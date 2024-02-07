/**
 * \brief
 * \author Vladimir Petrigo
 */
#include "median_filter.h"

#include <limits.h>
#include <stdlib.h>

static int unsigned_comp(const void *a, const void *b)
{
    const unsigned a_val = *(const unsigned *)a;
    const unsigned b_val = *(const unsigned *)b;

    if (a_val > b_val) {
        return 1;
    }
    else if (a_val < b_val) {
        return -1;
    }

    return 0;
}

bool median_filter_init(struct median_filter *filter, unsigned *buffer, size_t buffer_size)
{
    if (filter == NULL || buffer == NULL || buffer_size <= 1) {
        return false;
    }

    filter->buffer = buffer;
    filter->buffer_size = buffer_size;

    for (size_t i = 0; i < buffer_size; ++i) {
        filter->buffer[i] = UINT_MAX;
    }

    filter->current_pos = 0;

    return true;
}

bool median_filter_insert_number(struct median_filter *filter, unsigned sample)
{
    if (filter == NULL) {
        return false;
    }

    filter->buffer[filter->current_pos] = sample;
    filter->current_pos = (filter->current_pos + 1) % filter->buffer_size;
    qsort(filter->buffer, filter->buffer_size, sizeof(filter->buffer[0]), unsigned_comp);

    return true;
}

unsigned median_filter_get_median(const struct median_filter *filter)
{
    return filter->buffer[filter->buffer_size / 2];
}
