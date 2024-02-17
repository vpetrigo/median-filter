/**
 * \file
 * \brief
 * \author Vladimir Petrigo
 */
#include "median_filter.h"

#include <stdio.h>

int main(void)
{
    MEDIAN_FILTER_NEW(filter, uint32_t);
    MEDIAN_FILTER_BUFFER_NEW(buffer, 3, uint32_t);

    median_filter_init(&filter, buffer, 3);
    median_filter_insert_value(&filter, 3U);
    median_filter_insert_value(&filter, 2U);
    median_filter_insert_value(&filter, 1U);
    printf("Value: %u\n", median_filter_get_median(&filter));

    return 0;
}
