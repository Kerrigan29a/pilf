#ifndef __PILF_MINION__LINK_H__
#define __PILF_MINION__LINK_H__


/*******************************************************************************
 * Sockets list
 ***/

typedef struct _message {
    char * msg;
    struct _message *next, *prev;
} minion_message_t;

typedef struct _socket {
    int fd;
    int eid;
    minion_message_t * msg_queue;
    struct _socket *next, *prev;
} minion_socket_t;


#endif /* __PILF_MINION__LINK_H__ */