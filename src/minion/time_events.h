#ifndef __PILF_MINION__TIME_EVENTS_H__
#define __PILF_MINION__TIME_EVENTS_H__

#include "events.h"

/*******************************************************************************
 * Link event
 ***/
typedef enum _time_event_type  {
    MINION_TIME_STEP_EVENT =  MINION_TIME_EVENT | 0x1,
    MINION_TIME_WAKEUP_EVENT = MINION_TIME_EVENT | 0x2,
} minion_time_event_type_e;

typedef struct _time_event {
    MINION_BASIC_EVENT_FIELDS(_time_event);
} minion_time_event_t;

#define events_init_time_event(ctx, event, type) do { \
    ASSERT_NNULL(event); \
    ASSERT_BITS_IN(MINION_TIME_EVENT, (type)); \
    events_init_basic_event(ctx, event, type); \
} while(0)

#define events_finalize_time_event(ctx, event) do { \
    ASSERT_NNULL(event); \
    ASSERT_BITS_IN(MINION_TIME_EVENT, (event)->type); \
    events_finalize_basic_event(ctx, event); \
} while(0)

/*******************************************************************************
 * Link listener
 ***/
#define events_init_time_listener(ctx, listener, event_type, handler) do { \
    ASSERT_BITS_IN(MINION_TIME_EVENT, event_type); \
    events_init_basic_listener(ctx, listener, event_type, handler); \
} while(0)


#endif /* __PILF_MINION__TIME_EVENTS_H__ */
