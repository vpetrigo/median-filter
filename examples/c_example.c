/**
 * \file
 * \brief
 * \author
 */
#include "median_filter.h"

#include <stdio.h>

int main(void)
{
    struct median_filter_unsigned filter;
    unsigned buffer[3];

    median_filter_init(&filter, buffer, 3);
    median_filter_insert_number(&filter, 3U);
    median_filter_insert_number(&filter, 2U);
    median_filter_insert_number(&filter, 1U);

    printf("Value: %u\n", median_filter_get_median(&filter));

    return 0;
}