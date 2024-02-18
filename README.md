Median filter
=============

[![Test](https://github.com/vpetrigo/median-filter/actions/workflows/test.yml/badge.svg)](https://github.com/vpetrigo/median-filter/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/vpetrigo/median-filter/graph/badge.svg?token=kqWLifLSB6)](https://codecov.io/gh/vpetrigo/median-filter)
![GitHub Release](https://img.shields.io/github/v/release/vpetrigo/median-filter)

Window 1-D median filter implementation suitable for embedded systems applications. Implementation contains generic
functions for the following types:

- `float`
- `double`
- `uint8/16/32/64`
- `int8/16/32/64`

The implementation is based on [Phil Ekstrom's algorithm](https://www.embedded.com/better-than-average/) with an update 
which allow adding any value within type's range. Original algorithm assumes that there is a special "smallest" value
(usually the lower bound of an integer type used) at the end of a window.

# Example

---------

```c
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
```

# Contribution

--------------

Contributions are always welcome! If you have an idea, it's best to float it by me before working on it to ensure no
effort is wasted. If there's already an open issue for it, knock yourself out.

# License

---------

This project is licensed under:

- [Apache License, Version 2.0](LICENSE.md)

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in time by you, as
defined in the Apache-2.0 license, shall be licensed as above, without any additional terms or conditions.
