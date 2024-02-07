/**
 * \brief
 * \author Vladimir Petrigo
 */
#include "median_filter.h"

#include <stdlib.h>

// PRIVATE FUNCTION DEFINITIONS

#define GENERATE_COMPARE_FUNCTION(ty)                                                                                  \
    static int ty##_comp(const void *a, const void *b)                                                                 \
    {                                                                                                                  \
        const ty a_val = *(const ty *)a;                                                                               \
        const ty b_val = *(const ty *)b;                                                                               \
        if (a_val > b_val) {                                                                                           \
            return 1;                                                                                                  \
        }                                                                                                              \
        else if (a_val < b_val) {                                                                                      \
            return -1;                                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        return 0;                                                                                                      \
    }

GENERATE_COMPARE_FUNCTION(unsigned)
GENERATE_COMPARE_FUNCTION(int)
GENERATE_COMPARE_FUNCTION(float)
GENERATE_COMPARE_FUNCTION(uint8_t)
GENERATE_COMPARE_FUNCTION(uint16_t)
GENERATE_COMPARE_FUNCTION(uint32_t)
GENERATE_COMPARE_FUNCTION(uint64_t)
GENERATE_COMPARE_FUNCTION(int8_t)
GENERATE_COMPARE_FUNCTION(int16_t)
GENERATE_COMPARE_FUNCTION(int32_t)
GENERATE_COMPARE_FUNCTION(int64_t)

#define INIT_BUFFER(filter, buffer, buffer_size)                                                                       \
    do {                                                                                                               \
        (filter)->buffer = (buffer);                                                                                   \
        (filter)->buffer_size = (buffer_size);                                                                         \
        (filter)->current_pos = 0;                                                                                     \
        (filter)->size = 0;                                                                                            \
    } while (0)

static inline bool args_check(void *filter, void *buffer, size_t buffer_size)
{
    return filter != NULL && buffer != NULL && buffer_size > 1;
}

// PUBLIC FUNCTION DEFINITIONS

#define DEFINE_MEDIAN_INIT(ty)                                                                                         \
    bool median_filter_init_##ty(struct median_filter_##ty *filter, ty *buffer, size_t buffer_size)                    \
    {                                                                                                                  \
        if (!args_check(filter, buffer, buffer_size)) {                                                                \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        INIT_BUFFER(filter, buffer, buffer_size);                                                                      \
        return true;                                                                                                   \
    }

DEFINE_MEDIAN_INIT(unsigned)
DEFINE_MEDIAN_INIT(int)
DEFINE_MEDIAN_INIT(float)
DEFINE_MEDIAN_INIT(uint8_t)
DEFINE_MEDIAN_INIT(uint16_t)
DEFINE_MEDIAN_INIT(uint32_t)
DEFINE_MEDIAN_INIT(uint64_t)
DEFINE_MEDIAN_INIT(int8_t)
DEFINE_MEDIAN_INIT(int16_t)
DEFINE_MEDIAN_INIT(int32_t)
DEFINE_MEDIAN_INIT(int64_t)

#define DEFINE_MEDIAN_INSERT_NUMBER(ty)                                                                                \
    bool median_filter_insert_number_##ty(struct median_filter_##ty *filter, ty sample)                                \
    {                                                                                                                  \
        if (filter == NULL) {                                                                                          \
            return false;                                                                                              \
        }                                                                                                              \
        filter->buffer[filter->current_pos] = sample;                                                                  \
        filter->current_pos = (filter->current_pos + 1) % filter->buffer_size;                                         \
                                                                                                                       \
        if (filter->size != filter->buffer_size) {                                                                     \
            ++filter->size;                                                                                            \
        }                                                                                                              \
                                                                                                                       \
        qsort(filter->buffer, filter->size, sizeof(filter->buffer[0]), ty##_comp);                                     \
                                                                                                                       \
        return true;                                                                                                   \
    }

DEFINE_MEDIAN_INSERT_NUMBER(unsigned)
DEFINE_MEDIAN_INSERT_NUMBER(int)
DEFINE_MEDIAN_INSERT_NUMBER(float)
DEFINE_MEDIAN_INSERT_NUMBER(uint8_t)
DEFINE_MEDIAN_INSERT_NUMBER(uint16_t)
DEFINE_MEDIAN_INSERT_NUMBER(uint32_t)
DEFINE_MEDIAN_INSERT_NUMBER(uint64_t)
DEFINE_MEDIAN_INSERT_NUMBER(int8_t)
DEFINE_MEDIAN_INSERT_NUMBER(int16_t)
DEFINE_MEDIAN_INSERT_NUMBER(int32_t)
DEFINE_MEDIAN_INSERT_NUMBER(int64_t)

#define DEFINE_MEDIAN_GET(ty)                                                                                          \
    ty median_filter_get_median_##ty(const struct median_filter_##ty *filter)                                          \
    {                                                                                                                  \
        return filter->buffer[filter->size / 2];                                                                       \
    }

DEFINE_MEDIAN_GET(unsigned)
DEFINE_MEDIAN_GET(int)
DEFINE_MEDIAN_GET(float)
DEFINE_MEDIAN_GET(uint8_t)
DEFINE_MEDIAN_GET(uint16_t)
DEFINE_MEDIAN_GET(uint32_t)
DEFINE_MEDIAN_GET(uint64_t)
DEFINE_MEDIAN_GET(int8_t)
DEFINE_MEDIAN_GET(int16_t)
DEFINE_MEDIAN_GET(int32_t)
DEFINE_MEDIAN_GET(int64_t)
