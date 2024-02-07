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
#include <stdint.h>

#define MEDIAN_FILTER_DECLARE_STRUCT(ty)                                                                               \
    struct median_filter_##ty {                                                                                        \
        ty *buffer;                                                                                                    \
        size_t buffer_size;                                                                                            \
        size_t current_pos;                                                                                            \
        size_t size;                                                                                                   \
    }

MEDIAN_FILTER_DECLARE_STRUCT(unsigned);
MEDIAN_FILTER_DECLARE_STRUCT(int);
MEDIAN_FILTER_DECLARE_STRUCT(float);
MEDIAN_FILTER_DECLARE_STRUCT(uint8_t);
MEDIAN_FILTER_DECLARE_STRUCT(uint16_t);
MEDIAN_FILTER_DECLARE_STRUCT(uint32_t);
MEDIAN_FILTER_DECLARE_STRUCT(uint64_t);
MEDIAN_FILTER_DECLARE_STRUCT(int8_t);
MEDIAN_FILTER_DECLARE_STRUCT(int16_t);
MEDIAN_FILTER_DECLARE_STRUCT(int32_t);
MEDIAN_FILTER_DECLARE_STRUCT(int64_t);

#define MEDIAN_FILTER_DECLARE_INIT(ty)                                                                                 \
    bool median_filter_init_##ty(struct median_filter_##ty *filter, ty *buffer, size_t buffer_size)

MEDIAN_FILTER_DECLARE_INIT(unsigned);
MEDIAN_FILTER_DECLARE_INIT(int);
MEDIAN_FILTER_DECLARE_INIT(float);
MEDIAN_FILTER_DECLARE_INIT(uint8_t);
MEDIAN_FILTER_DECLARE_INIT(uint16_t);
MEDIAN_FILTER_DECLARE_INIT(uint32_t);
MEDIAN_FILTER_DECLARE_INIT(uint64_t);
MEDIAN_FILTER_DECLARE_INIT(int8_t);
MEDIAN_FILTER_DECLARE_INIT(int16_t);
MEDIAN_FILTER_DECLARE_INIT(int32_t);
MEDIAN_FILTER_DECLARE_INIT(int64_t);

#define MEDIAN_FILTER_DECLARE_INSERT(ty)                                                                               \
    bool median_filter_insert_number_##ty(struct median_filter_##ty *filter, ty sample)

MEDIAN_FILTER_DECLARE_INSERT(unsigned);
MEDIAN_FILTER_DECLARE_INSERT(int);
MEDIAN_FILTER_DECLARE_INSERT(float);
MEDIAN_FILTER_DECLARE_INSERT(uint8_t);
MEDIAN_FILTER_DECLARE_INSERT(uint16_t);
MEDIAN_FILTER_DECLARE_INSERT(uint32_t);
MEDIAN_FILTER_DECLARE_INSERT(uint64_t);
MEDIAN_FILTER_DECLARE_INSERT(int8_t);
MEDIAN_FILTER_DECLARE_INSERT(int16_t);
MEDIAN_FILTER_DECLARE_INSERT(int32_t);
MEDIAN_FILTER_DECLARE_INSERT(int64_t);

#define MEDIAN_FILTER_DECLARE_GET(ty) ty median_filter_get_median_##ty(const struct median_filter_##ty *filter)

MEDIAN_FILTER_DECLARE_GET(unsigned);
MEDIAN_FILTER_DECLARE_GET(int);
MEDIAN_FILTER_DECLARE_GET(float);
MEDIAN_FILTER_DECLARE_GET(uint8_t);
MEDIAN_FILTER_DECLARE_GET(uint16_t);
MEDIAN_FILTER_DECLARE_GET(uint32_t);
MEDIAN_FILTER_DECLARE_GET(uint64_t);
MEDIAN_FILTER_DECLARE_GET(int8_t);
MEDIAN_FILTER_DECLARE_GET(int16_t);
MEDIAN_FILTER_DECLARE_GET(int32_t);
MEDIAN_FILTER_DECLARE_GET(int64_t);

#ifndef __cplusplus
#define median_filter_gen_line_struct(ty, fn) struct median_filter_##ty * : fn##_##ty

#define median_filter_gen_line(ty, fn)                                                                                 \
    ty:                                                                                                                \
    fn##_##ty

#define median_filter_gen_line_ptr(ty, fn) ty * : fn##_##ty
// TODO: int32/uint32 specific functions based on architecture
#define median_filter_init(filter, buf, size)                                                                          \
    _Generic((buf),                                                                                                    \
        median_filter_gen_line_ptr(unsigned, median_filter_init),                                                      \
        median_filter_gen_line_ptr(int, median_filter_init),                                                           \
        median_filter_gen_line_ptr(float, median_filter_init),                                                         \
        median_filter_gen_line_ptr(uint8_t, median_filter_init),                                                       \
        median_filter_gen_line_ptr(uint16_t, median_filter_init),                                                      \
        median_filter_gen_line_ptr(uint64_t, median_filter_init),                                                      \
        median_filter_gen_line_ptr(int8_t, median_filter_init),                                                        \
        median_filter_gen_line_ptr(int16_t, median_filter_init),                                                       \
        median_filter_gen_line_ptr(int64_t, median_filter_init))(filter, buf, size)

#define median_filter_insert_number(filter, sample)                                                                    \
    _Generic((sample),                                                                                                 \
        median_filter_gen_line(unsigned, median_filter_insert_number),                                                 \
        median_filter_gen_line(int, median_filter_insert_number),                                                      \
        median_filter_gen_line(float, median_filter_insert_number),                                                    \
        median_filter_gen_line(uint8_t, median_filter_insert_number),                                                  \
        median_filter_gen_line(uint16_t, median_filter_insert_number),                                                 \
        median_filter_gen_line(uint64_t, median_filter_insert_number),                                                 \
        median_filter_gen_line(int8_t, median_filter_insert_number),                                                   \
        median_filter_gen_line(int16_t, median_filter_insert_number),                                                  \
        median_filter_gen_line(int64_t, median_filter_insert_number))(filter, sample)

#define median_filter_get_median(filter)                                                                               \
    _Generic((filter),                                                                                                 \
        median_filter_gen_line_struct(unsigned, median_filter_get_median),                     \
        median_filter_gen_line_struct(int, median_filter_get_median),                          \
        median_filter_gen_line_struct(float, median_filter_get_median),                        \
        median_filter_gen_line_struct(uint8_t, median_filter_get_median),                      \
        median_filter_gen_line_struct(uint16_t, median_filter_get_median),                     \
        median_filter_gen_line_struct(uint64_t, median_filter_get_median),                     \
        median_filter_gen_line_struct(int8_t, median_filter_get_median),                       \
        median_filter_gen_line_struct(int16_t, median_filter_get_median),                      \
        median_filter_gen_line_struct(int64_t, median_filter_get_median))(filter)
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define MEDIAN_FILTER_OVERLOAD(ty)                                                                                     \
    static inline bool median_filter_init(struct median_filter_##ty *filter, ty *buffer, size_t buffer_size)           \
    {                                                                                                                  \
        return median_filter_init_##ty(filter, buffer, buffer_size);                                                   \
    }

MEDIAN_FILTER_OVERLOAD(unsigned)
MEDIAN_FILTER_OVERLOAD(int)
MEDIAN_FILTER_OVERLOAD(float)
MEDIAN_FILTER_OVERLOAD(uint8_t)
MEDIAN_FILTER_OVERLOAD(uint16_t)
MEDIAN_FILTER_OVERLOAD(uint32_t)
MEDIAN_FILTER_OVERLOAD(uint64_t)
MEDIAN_FILTER_OVERLOAD(int8_t)
MEDIAN_FILTER_OVERLOAD(int16_t)
MEDIAN_FILTER_OVERLOAD(int32_t)
MEDIAN_FILTER_OVERLOAD(int64_t)
#endif

#endif /* MEDIAN_FILTER_H */
