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

#define MEDIAN_FILTER_DECLARE_NODE_STRUCT(ty)                                                                          \
    struct median_filter_node_##ty {                                                                                   \
        struct median_filter_node_##ty *prev;                                                                          \
        struct median_filter_node_##ty *next;                                                                          \
        ty value;                                                                                                      \
    };

MEDIAN_FILTER_DECLARE_NODE_STRUCT(float)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(double)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(uint8_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(uint16_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(uint32_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(uint64_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(int8_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(int16_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(int32_t)
MEDIAN_FILTER_DECLARE_NODE_STRUCT(int64_t)

#define MEDIAN_FILTER_DECLARE_STRUCT(ty)                                                                               \
    struct median_filter_##ty {                                                                                        \
        struct median_filter_node_##ty *buffer;                                                                        \
        struct median_filter_node_##ty *smallest;                                                                      \
        struct median_filter_node_##ty *median;                                                                        \
        size_t current_pos;                                                                                            \
        size_t size;                                                                                                   \
    }

MEDIAN_FILTER_DECLARE_STRUCT(float);
MEDIAN_FILTER_DECLARE_STRUCT(double);
MEDIAN_FILTER_DECLARE_STRUCT(uint8_t);
MEDIAN_FILTER_DECLARE_STRUCT(uint16_t);
MEDIAN_FILTER_DECLARE_STRUCT(uint32_t);
MEDIAN_FILTER_DECLARE_STRUCT(uint64_t);
MEDIAN_FILTER_DECLARE_STRUCT(int8_t);
MEDIAN_FILTER_DECLARE_STRUCT(int16_t);
MEDIAN_FILTER_DECLARE_STRUCT(int32_t);
MEDIAN_FILTER_DECLARE_STRUCT(int64_t);

#define MEDIAN_FILTER_DECLARE_INIT(ty)                                                                                 \
    bool median_filter_init_##ty(struct median_filter_##ty *filter, struct median_filter_node_##ty *buffer,            \
                                 size_t buffer_size)

MEDIAN_FILTER_DECLARE_INIT(float);
MEDIAN_FILTER_DECLARE_INIT(double);
MEDIAN_FILTER_DECLARE_INIT(uint8_t);
MEDIAN_FILTER_DECLARE_INIT(uint16_t);
MEDIAN_FILTER_DECLARE_INIT(uint32_t);
MEDIAN_FILTER_DECLARE_INIT(uint64_t);
MEDIAN_FILTER_DECLARE_INIT(int8_t);
MEDIAN_FILTER_DECLARE_INIT(int16_t);
MEDIAN_FILTER_DECLARE_INIT(int32_t);
MEDIAN_FILTER_DECLARE_INIT(int64_t);

#define MEDIAN_FILTER_DECLARE_INSERT(ty)                                                                               \
    bool median_filter_insert_value_##ty(struct median_filter_##ty *filter, ty sample)

MEDIAN_FILTER_DECLARE_INSERT(float);
MEDIAN_FILTER_DECLARE_INSERT(double);
MEDIAN_FILTER_DECLARE_INSERT(uint8_t);
MEDIAN_FILTER_DECLARE_INSERT(uint16_t);
MEDIAN_FILTER_DECLARE_INSERT(uint32_t);
MEDIAN_FILTER_DECLARE_INSERT(uint64_t);
MEDIAN_FILTER_DECLARE_INSERT(int8_t);
MEDIAN_FILTER_DECLARE_INSERT(int16_t);
MEDIAN_FILTER_DECLARE_INSERT(int32_t);
MEDIAN_FILTER_DECLARE_INSERT(int64_t);

#define MEDIAN_FILTER_DECLARE_GET(ty) ty median_filter_get_median_##ty(const struct median_filter_##ty *filter)

MEDIAN_FILTER_DECLARE_GET(float);
MEDIAN_FILTER_DECLARE_GET(double);
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

#define median_filter_init(filter, buf, size)                                                                          \
    _Generic((buf),                                                                                                    \
        median_filter_gen_line_ptr(float, median_filter_init),                                                         \
        median_filter_gen_line_ptr(double, median_filter_init),                                                        \
        median_filter_gen_line_ptr(uint8_t, median_filter_init),                                                       \
        median_filter_gen_line_ptr(uint16_t, median_filter_init),                                                      \
        median_filter_gen_line_ptr(uint32_t, median_filter_init),                                                      \
        median_filter_gen_line_ptr(uint64_t, median_filter_init),                                                      \
        median_filter_gen_line_ptr(int8_t, median_filter_init),                                                        \
        median_filter_gen_line_ptr(int16_t, median_filter_init),                                                       \
        median_filter_gen_line_ptr(int32_t, median_filter_init),                                                       \
        median_filter_gen_line_ptr(int64_t, median_filter_init))(filter, buf, size)

#define median_filter_insert_number(filter, sample)                                                                    \
    _Generic((sample),                                                                                                 \
        median_filter_gen_line(float, median_filter_insert_number),                                                    \
        median_filter_gen_line(double, median_filter_insert_number),                                                   \
        median_filter_gen_line(uint8_t, median_filter_insert_number),                                                  \
        median_filter_gen_line(uint16_t, median_filter_insert_number),                                                 \
        median_filter_gen_line(uint32_t, median_filter_insert_number),                                                 \
        median_filter_gen_line(uint64_t, median_filter_insert_number),                                                 \
        median_filter_gen_line(int8_t, median_filter_insert_number),                                                   \
        median_filter_gen_line(int16_t, median_filter_insert_number),                                                  \
        median_filter_gen_line(int32_t, median_filter_insert_number),                                                  \
        median_filter_gen_line(int64_t, median_filter_insert_number))(filter, sample)

#define median_filter_get_median(filter)                                                                               \
    _Generic((filter),                                                                                                 \
        median_filter_gen_line_struct(float, median_filter_get_median),                                                \
        median_filter_gen_line_struct(double, median_filter_get_median),                                               \
        median_filter_gen_line_struct(uint8_t, median_filter_get_median),                                              \
        median_filter_gen_line_struct(uint16_t, median_filter_get_median),                                             \
        median_filter_gen_line_struct(uint32_t, median_filter_get_median),                                             \
        median_filter_gen_line_struct(uint64_t, median_filter_get_median),                                             \
        median_filter_gen_line_struct(int8_t, median_filter_get_median),                                               \
        median_filter_gen_line_struct(int16_t, median_filter_get_median),                                              \
        median_filter_gen_line_struct(int32_t, median_filter_get_median),                                              \
        median_filter_gen_line_struct(int64_t, median_filter_get_median))(filter)
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define MEDIAN_FILTER_INIT_OVERLOAD(ty)                                                                                \
    static inline bool median_filter_init(struct median_filter_##ty *filter, struct median_filter_node_##ty *buffer,   \
                                          size_t buffer_size)                                                          \
    {                                                                                                                  \
        return median_filter_init_##ty(filter, buffer, buffer_size);                                                   \
    }

MEDIAN_FILTER_INIT_OVERLOAD(float)
MEDIAN_FILTER_INIT_OVERLOAD(double)
MEDIAN_FILTER_INIT_OVERLOAD(uint8_t)
MEDIAN_FILTER_INIT_OVERLOAD(uint16_t)
MEDIAN_FILTER_INIT_OVERLOAD(uint32_t)
MEDIAN_FILTER_INIT_OVERLOAD(uint64_t)
MEDIAN_FILTER_INIT_OVERLOAD(int8_t)
MEDIAN_FILTER_INIT_OVERLOAD(int16_t)
MEDIAN_FILTER_INIT_OVERLOAD(int32_t)
MEDIAN_FILTER_INIT_OVERLOAD(int64_t)

#define MEDIAN_FILTER_INSERT_NUMBER(ty)                                                                                \
    static inline bool median_filter_insert_value(struct median_filter_##ty *filter, ty sample)                        \
    {                                                                                                                  \
        return median_filter_insert_value_##ty(filter, sample);                                                        \
    }

MEDIAN_FILTER_INSERT_NUMBER(float)
MEDIAN_FILTER_INSERT_NUMBER(double)
MEDIAN_FILTER_INSERT_NUMBER(uint8_t)
MEDIAN_FILTER_INSERT_NUMBER(uint16_t)
MEDIAN_FILTER_INSERT_NUMBER(uint32_t)
MEDIAN_FILTER_INSERT_NUMBER(uint64_t)
MEDIAN_FILTER_INSERT_NUMBER(int8_t)
MEDIAN_FILTER_INSERT_NUMBER(int16_t)
MEDIAN_FILTER_INSERT_NUMBER(int32_t)
MEDIAN_FILTER_INSERT_NUMBER(int64_t)

#define MEDIAN_FILTER_GET_MEDIAN(ty)                                                                                   \
    static inline ty median_filter_get_median(struct median_filter_##ty *filter)                                       \
    {                                                                                                                  \
        return median_filter_get_median_##ty(filter);                                                                  \
    }

MEDIAN_FILTER_GET_MEDIAN(float)
MEDIAN_FILTER_GET_MEDIAN(double)
MEDIAN_FILTER_GET_MEDIAN(uint8_t)
MEDIAN_FILTER_GET_MEDIAN(uint16_t)
MEDIAN_FILTER_GET_MEDIAN(uint32_t)
MEDIAN_FILTER_GET_MEDIAN(uint64_t)
MEDIAN_FILTER_GET_MEDIAN(int8_t)
MEDIAN_FILTER_GET_MEDIAN(int16_t)
MEDIAN_FILTER_GET_MEDIAN(int32_t)
MEDIAN_FILTER_GET_MEDIAN(int64_t)

#endif

#endif /* MEDIAN_FILTER_H */
