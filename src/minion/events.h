#ifndef __PILF_MINION__EVENTS_H__
#define __PILF_MINION__EVENTS_H__

#include <time.h>

#include <uthash/utlist.h>

#include "minion_internals.h"


typedef enum _event_type {
    MINION_TIME_EVENT =     0x0100,
    MINION_LINK_EVENT =     0x0200,
    MINION_ENGINE_EVENT =   0x0400,
} minion_event_type_e;

/*******************************************************************************
 * Basic event
 ***/

#define MINION_BASIC_EVENT_FIELDS(struct_type) struct { \
    unsigned int type; \
    time_t timestamp; \
    struct struct_type * next; \
    struct struct_type * prev; \
}

typedef struct _basic_event {
    MINION_BASIC_EVENT_FIELDS(_basic_event);
} minion_basic_event_t;

#define events_init_basic_event(ctx, event, _type) do { \
    ASSERT_NNULL(event); \
    ASSERT_TRUE((_type) > 0); \
    (event)->type = (_type); \
    (event)->timestamp = -1; \
    (event)->next = NULL; \
    (event)->prev = NULL; \
} while(0)

#define events_finalize_basic_event(ctx, event) do { \
    ASSERT_NNULL(event); \
    (event)->type = 0; \
    (event)->timestamp = -1; \
    (event)->next = NULL; \
    (event)->prev = NULL; \
} while(0)


/*******************************************************************************
 * Basic listener
 ***/

struct _context;

typedef int (* minion_event_handler_t) (struct _context * const, minion_basic_event_t * const);

typedef struct _listener {
    unsigned int event_type;
    minion_event_handler_t handler;
    struct _listener *next, *prev;
} minion_listener_t;

#define events_init_basic_listener(ctx, listener, _event_type, _handler) do { \
    ASSERT_NNULL(listener); \
    ASSERT_TRUE((_event_type) > 0); \
    ASSERT_NNULL(_handler); \
    (listener)->event_type = (_event_type); \
    (listener)->handler = (minion_event_handler_t) (_handler); \
    (listener)->next = NULL; \
    (listener)->prev = NULL; \
} while(0)

/*******************************************************************************
 * Useful macros
 ***/

#define EVENTS_BIND_NEW_LISTENER(T, ctx, event_type, handler) do { \
    minion_listener_t *l; \
    ASSERT_NNULL(handler); \
    l = (minion_listener_t *) minion_secure_malloc(ctx, \
        sizeof(minion_listener_t)); \
    CHECK_NNULL_R(l, __LINE__); \
    events_init_##T##_listener(ctx, l, event_type, handler); \
    events_bind(ctx, l); \
} while (0)


#define EVENTS_NOTIFY_NEW_EVENT(T, ctx, type, ...) do { \
    minion_##T##_event_t *e; \
    e = (minion_##T##_event_t *) minion_secure_malloc(ctx, \
        sizeof(minion_##T##_event_t)); \
    CHECK_NNULL_R(e, __LINE__); \
    events_init_##T##_event(ctx, e, type, __VA_ARGS__); \
    events_notify(ctx, (minion_basic_event_t *) e); \
} while (0)


#endif /* __PILF_MINION__EVENTS_H__ */
