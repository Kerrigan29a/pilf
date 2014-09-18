#ifndef __PILF_MINION__LINK_EVENTS_H__
#define __PILF_MINION__LINK_EVENTS_H__

#include "events.h"

/*******************************************************************************
 * Link event
 ***/
typedef enum _link_event_type  {
    MINION_LINK_POLLIN_EVENT =  MINION_LINK_EVENT | 0x1,
    MINION_LINK_POLLOUT_EVENT = MINION_LINK_EVENT | 0x2,
    MINION_LINK_INMSG_EVENT =   MINION_LINK_EVENT | 0x4,
    MINION_LINK_OUTMSG_EVENT =  MINION_LINK_EVENT | 0x8,
} minion_link_event_type_e;

typedef struct _link_event {
    MINION_BASIC_EVENT_FIELDS(_link_event);
    int fd;
    char * msg;
} minion_link_event_t;

#define events_init_link_event(ctx, event, type, _fd, _msg, msg_len) do { \
    char *new_msg; \
    ASSERT_NNULL(event); \
    ASSERT_BITS_IN(MINION_LINK_EVENT, (type)); \
    ASSERT_TRUE((_fd) >= 0); \
    events_init_basic_event(ctx, event, type); \
    (event)->fd = (_fd); \
    if (IS_NULL(_msg)) { \
        new_msg = NULL; \
        ASSERT_ZERO(msg_len); \
    } else { \
        new_msg = minion_strndup(ctx, _msg, msg_len); \
        CHECK_STR_R(new_msg, __LINE__); \
    } \
    (event)->msg = new_msg; \
} while(0)

#define events_finalize_link_event(ctx, event) do { \
    ASSERT_NNULL(event); \
    ASSERT_BITS_IN(MINION_LINK_EVENT, (event)->type); \
    events_finalize_basic_event(ctx, event); \
    (event)->fd = -1; \
    if (IS_STR((event)->msg)) { \
        minion_secure_free(ctx, (event)->msg, strlen((event)->msg)); \
    } \
} while(0)


/*******************************************************************************
 * Link listener
 ***/
#define events_init_link_listener(ctx, listener, event_type, handler) do { \
    ASSERT_BITS_IN(MINION_LINK_EVENT, event_type); \
    events_init_basic_listener(ctx, listener, event_type, handler); \
} while(0)


#endif /* __PILF_MINION__LINK_EVENTS_H__ */
