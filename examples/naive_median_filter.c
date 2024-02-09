/**
 * \file
 * \brief
 * \author
 */

#include "naive_median_filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare_unsigned(const void *a, const void *b)
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

void mf_init(struct median_filter *filter, unsigned *main_buffer, unsigned *helper_buffer, size_t buffer_size)
{
    filter->buffer = main_buffer;
    filter->sorted_buffer = helper_buffer;
    filter->size = 0;
    filter->current_pos = 0;
    filter->buffer_size = buffer_size;
    memset(filter->buffer, 0, filter->buffer_size * sizeof(unsigned));
}

void mf_insert_value(struct median_filter *filter, unsigned value)
{
    filter->buffer[filter->current_pos] = value;
    filter->current_pos = (filter->current_pos + 1) % filter->buffer_size;

    if (filter->size != filter->buffer_size) {
        ++filter->size;
    }

    memcpy(filter->sorted_buffer, filter->buffer, filter->size * sizeof(filter->sorted_buffer[0]));
    qsort(filter->sorted_buffer, filter->size, sizeof(unsigned), compare_unsigned);
}

void mf_debug(const struct median_filter *filter)
{
    for (size_t i = 0; i < filter->buffer_size; ++i) {
        printf("buffer[%zu]: %u\n", i, filter->buffer[i]);
        printf("sorted buffer[%zu]: %u\n", i, filter->sorted_buffer[i]);
    }
}

unsigned mf_get_median(const struct median_filter *filter)
{
    if (filter->size == 1) {
        return filter->sorted_buffer[0];
    }
    else if (filter->size % 2 == 0) {
        return filter->sorted_buffer[filter->size / 2 - 1];
    }

    return filter->sorted_buffer[filter->size / 2];
}
