#ifndef __PILF_MINION__ENGINE_EVENTS_H__
#define __PILF_MINION__ENGINE_EVENTS_H__


#include "events.h"

/*******************************************************************************
 * Engine event
 ***/

typedef enum _engine_event_type {
    MINION_ENGINE_EXEC_EVENT =  MINION_ENGINE_EVENT | 0x1,
    MINION_ENGINE_DONE_EVENT =  MINION_ENGINE_EVENT | 0x2,
} minion_engine_event_type_e;

typedef struct _engine_event {
    MINION_BASIC_EVENT_FIELDS(_engine_event);
    char * buffer;
} minion_engine_event_t;

#define events_init_engine_event(ctx, event, type, buf, buf_len) do { \
    ASSERT_NNULL(event); \
    ASSERT_BITS_IN(MINION_ENGINE_EVENT, type); \
    ASSERT_STR(buf); \
    ASSERT_TRUE(buf_len); \
    events_init_basic_event(ctx, event, type); \
    (event)->buffer = pilf_strndup(buf, buf_len); \
} while(0)

#define events_finalize_engine_event(ctx, event) do { \
    ASSERT_NNULL(event); \
    ASSERT_BITS_IN(MINION_ENGINE_EVENT, (event)->type); \
    ASSERT_STR((event)->buffer); \
    events_finalize_basic_event(ctx, event); \
    pilf_secure_free((event)->buffer, strlen((event)->buffer)); \
} while(0)


/*******************************************************************************
 * Engine listener
 ***/
#define events_init_engine_listener(ctx, listener, event_type, handler) do { \
    ASSERT_BITS_IN(MINION_ENGINE_EVENT, event_type); \
    events_init_basic_listener(ctx, listener, event_type, handler); \
} while(0)


#endif /* __PILF_MINION__ENGINE_EVENTS_H__ */
