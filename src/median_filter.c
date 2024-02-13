/**
 * \brief
 * \author Vladimir Petrigo
 */
#include "median_filter.h"

#include <string.h>

// PRIVATE FUNCTION DEFINITIONS

#define GET_INSERT_POSITION(ty)                                                                                        \
    static inline size_t get_insert_position_##ty(const struct median_filter_##ty *filter)                             \
    {                                                                                                                  \
        return filter->current_pos;                                                                                    \
    }

GET_INSERT_POSITION(float)
GET_INSERT_POSITION(double)
GET_INSERT_POSITION(uint8_t)
GET_INSERT_POSITION(uint16_t)
GET_INSERT_POSITION(uint32_t)
GET_INSERT_POSITION(uint64_t)
GET_INSERT_POSITION(int8_t)
GET_INSERT_POSITION(int16_t)
GET_INSERT_POSITION(int32_t)
GET_INSERT_POSITION(int64_t)

#define GET_NEXT_POSITION(ty)                                                                                          \
    static inline size_t get_next_position_##ty(const struct median_filter_##ty *filter)                               \
    {                                                                                                                  \
        return (filter->current_pos + 1) % filter->size;                                                               \
    }

GET_NEXT_POSITION(float)
GET_NEXT_POSITION(double)
GET_NEXT_POSITION(uint8_t)
GET_NEXT_POSITION(uint16_t)
GET_NEXT_POSITION(uint32_t)
GET_NEXT_POSITION(uint64_t)
GET_NEXT_POSITION(int8_t)
GET_NEXT_POSITION(int16_t)
GET_NEXT_POSITION(int32_t)
GET_NEXT_POSITION(int64_t)

#define FIND_MEDIAN(ty)                                                                                                \
    static inline void find_median_##ty(struct median_filter_##ty *filter)                                             \
    {                                                                                                                  \
        struct median_filter_node_##ty *it = filter->smallest;                                                         \
                                                                                                                       \
        for (size_t i = 0; i < filter->size / 2 && it->next != NULL; ++i) {                                            \
            it = it->next;                                                                                             \
        }                                                                                                              \
                                                                                                                       \
        filter->median = it;                                                                                           \
    }

FIND_MEDIAN(float)
FIND_MEDIAN(double)
FIND_MEDIAN(uint8_t)
FIND_MEDIAN(uint16_t)
FIND_MEDIAN(uint32_t)
FIND_MEDIAN(uint64_t)
FIND_MEDIAN(int8_t)
FIND_MEDIAN(int16_t)
FIND_MEDIAN(int32_t)
FIND_MEDIAN(int64_t)

#define REMOVE_NODE_FROM_LIST(ty)                                                                                      \
    static inline void remove_node_from_list_##ty(struct median_filter_node_##ty *node)                                \
    {                                                                                                                  \
        if (node->prev != NULL) {                                                                                      \
            node->prev->next = node->next;                                                                             \
        }                                                                                                              \
                                                                                                                       \
        if (node->next != NULL) {                                                                                      \
            node->next->prev = node->prev;                                                                             \
        }                                                                                                              \
                                                                                                                       \
        node->prev = NULL;                                                                                             \
        node->next = NULL;                                                                                             \
    }

REMOVE_NODE_FROM_LIST(float)
REMOVE_NODE_FROM_LIST(double)
REMOVE_NODE_FROM_LIST(uint8_t)
REMOVE_NODE_FROM_LIST(uint16_t)
REMOVE_NODE_FROM_LIST(uint32_t)
REMOVE_NODE_FROM_LIST(uint64_t)
REMOVE_NODE_FROM_LIST(int8_t)
REMOVE_NODE_FROM_LIST(int16_t)
REMOVE_NODE_FROM_LIST(int32_t)
REMOVE_NODE_FROM_LIST(int64_t)

#define INSERT_AFTER_NODE(ty)                                                                                          \
    static inline void insert_after_node_##ty(struct median_filter_node_##ty *node,                                    \
                                              struct median_filter_node_##ty *insert)                                  \
    {                                                                                                                  \
        if (node->next != NULL) {                                                                                      \
            node->next->prev = insert;                                                                                 \
            insert->next = node->next;                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        node->next = insert;                                                                                           \
        insert->prev = node;                                                                                           \
    }

INSERT_AFTER_NODE(float)
INSERT_AFTER_NODE(double)
INSERT_AFTER_NODE(uint8_t)
INSERT_AFTER_NODE(uint16_t)
INSERT_AFTER_NODE(uint32_t)
INSERT_AFTER_NODE(uint64_t)
INSERT_AFTER_NODE(int8_t)
INSERT_AFTER_NODE(int16_t)
INSERT_AFTER_NODE(int32_t)
INSERT_AFTER_NODE(int64_t)

#define INSERT_BEFORE_NODE(ty)                                                                                         \
    static inline void insert_before_node_##ty(struct median_filter_node_##ty *node,                                   \
                                               struct median_filter_node_##ty *insert)                                 \
    {                                                                                                                  \
        if (node->prev != NULL) {                                                                                      \
            node->prev->next = insert;                                                                                 \
            insert->prev = node->prev;                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        insert->next = node;                                                                                           \
        node->prev = insert;                                                                                           \
    }

INSERT_BEFORE_NODE(float)
INSERT_BEFORE_NODE(double)
INSERT_BEFORE_NODE(uint8_t)
INSERT_BEFORE_NODE(uint16_t)
INSERT_BEFORE_NODE(uint32_t)
INSERT_BEFORE_NODE(uint64_t)
INSERT_BEFORE_NODE(int8_t)
INSERT_BEFORE_NODE(int16_t)
INSERT_BEFORE_NODE(int32_t)
INSERT_BEFORE_NODE(int64_t)

#define INIT_BUFFER(filter, buffer, buffer_size, ty)                                                                   \
    do {                                                                                                               \
        (filter)->smallest = NULL;                                                                                     \
        (filter)->median = NULL;                                                                                       \
        (filter)->buffer = buffer;                                                                                     \
        (filter)->size = buffer_size;                                                                                  \
        (filter)->current_pos = 0;                                                                                     \
        memset(filter->buffer, 0, filter->size * sizeof(struct median_filter_node_##ty));                              \
    } while (0)

static inline bool args_check(void *filter, void *buffer, size_t buffer_size)
{
    return filter != NULL && buffer != NULL && buffer_size > 1;
}

// PUBLIC FUNCTION DEFINITIONS

#define DEFINE_MEDIAN_INIT(ty)                                                                                         \
    bool median_filter_init_##ty(struct median_filter_##ty *filter, struct median_filter_node_##ty *buffer,            \
                                 size_t buffer_size)                                                                   \
    {                                                                                                                  \
        if (!args_check(filter, buffer, buffer_size)) {                                                                \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        INIT_BUFFER(filter, buffer, buffer_size, ty);                                                                  \
        return true;                                                                                                   \
    }

DEFINE_MEDIAN_INIT(float)
DEFINE_MEDIAN_INIT(double)
DEFINE_MEDIAN_INIT(uint8_t)
DEFINE_MEDIAN_INIT(uint16_t)
DEFINE_MEDIAN_INIT(uint32_t)
DEFINE_MEDIAN_INIT(uint64_t)
DEFINE_MEDIAN_INIT(int8_t)
DEFINE_MEDIAN_INIT(int16_t)
DEFINE_MEDIAN_INIT(int32_t)
DEFINE_MEDIAN_INIT(int64_t)

#define DEFINE_MEDIAN_INSERT_NUMBER(ty)                                                                                \
    bool median_filter_insert_value_##ty(struct median_filter_##ty *filter, ty value)                                  \
    {                                                                                                                  \
        if (filter == NULL) {                                                                                          \
            return false;                                                                                              \
        }                                                                                                              \
        const size_t insert_pos = get_insert_position_##ty(filter);                                                    \
        struct median_filter_node_##ty *insert = &filter->buffer[insert_pos];                                          \
                                                                                                                       \
        if (filter->smallest == NULL) {                                                                                \
            filter->smallest = insert;                                                                                 \
            insert->value = value;                                                                                     \
            filter->median = filter->smallest;                                                                         \
            goto exit;                                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        if (filter->smallest == insert) {                                                                              \
            filter->smallest = insert->next;                                                                           \
        }                                                                                                              \
                                                                                                                       \
        insert->value = value;                                                                                         \
        remove_node_from_list_##ty(insert);                                                                            \
                                                                                                                       \
        struct median_filter_node_##ty *current = filter->smallest;                                                    \
                                                                                                                       \
        while (current->next != NULL) {                                                                                \
            if (current->value > insert->value) {                                                                      \
                break;                                                                                                 \
            }                                                                                                          \
                                                                                                                       \
            current = current->next;                                                                                   \
        }                                                                                                              \
                                                                                                                       \
        if (current->value > insert->value) {                                                                          \
            if (current == filter->smallest) {                                                                         \
                filter->smallest = insert;                                                                             \
            }                                                                                                          \
                                                                                                                       \
            insert_before_node_##ty(current, insert);                                                                  \
        }                                                                                                              \
        else {                                                                                                         \
            insert_after_node_##ty(current, insert);                                                                   \
        }                                                                                                              \
                                                                                                                       \
    exit:                                                                                                              \
        filter->current_pos = get_next_position_##ty(filter);                                                          \
        find_median_##ty(filter);                                                                                      \
        return true;                                                                                                   \
    }

DEFINE_MEDIAN_INSERT_NUMBER(float)
DEFINE_MEDIAN_INSERT_NUMBER(double)
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
        return filter->median->value;                                                                                  \
    }

DEFINE_MEDIAN_GET(float)
DEFINE_MEDIAN_GET(double)
DEFINE_MEDIAN_GET(uint8_t)
DEFINE_MEDIAN_GET(uint16_t)
DEFINE_MEDIAN_GET(uint32_t)
DEFINE_MEDIAN_GET(uint64_t)
DEFINE_MEDIAN_GET(int8_t)
DEFINE_MEDIAN_GET(int16_t)
DEFINE_MEDIAN_GET(int32_t)
DEFINE_MEDIAN_GET(int64_t)
