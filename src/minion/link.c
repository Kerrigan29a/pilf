#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#include <uthash/utlist.h>

#include "utlist_utils.h"
#include "minion_internals.h"


#if MINION_TRACE_LINK
#define MINION_TRACE_FUNCTION() TRACE_FUNCTION_CALL(ctx, __func__, __FILE__, __LINE__)
#else
#define MINION_TRACE_FUNCTION() ((void)0)
#endif


PILF_PRIVATE int _handle_pollin_event(minion_context_t * const ctx,
    minion_link_event_t * const event)
{
    char * buf = NULL;
    int bytes;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_EQ(MINION_LINK_POLLIN_EVENT, event->type);
    ASSERT_TRUE(event->fd >= 0);

    bytes = nn_recv(event->fd, &buf, NN_MSG, NN_DONTWAIT);
    if (bytes < 0) {
        return __LINE__;
    }

    EVENTS_NOTIFY_NEW_EVENT(link, ctx, MINION_LINK_INMSG_EVENT, event->fd, buf, bytes);

    nn_freemsg(buf);

    return 0;
}

PILF_PRIVATE int _handle_outmsg_event(minion_context_t * const ctx,
    minion_link_event_t * const event)
{
    minion_socket_t * socket;
    minion_message_t * message;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_EQ(MINION_LINK_OUTMSG_EVENT, event->type);
    ASSERT_TRUE(event->fd >= 0);
    ASSERT_STR(event->msg);

    debug(ctx, "[events] (%zu) OUT_MSG = %s", strlen(event->msg), event->msg);

    /* Search the socket */
    DL_SEARCH_SCALAR(ctx->sockets, socket, fd, event->fd);
    CHECK_NNULL_R(socket, __LINE__);

    /* Copy the message */
    DL_NEW_NODE(ctx, message, minion_message_t);
    message->msg = minion_strdup(ctx, event->msg);
    CHECK_STR_R(message->msg, __LINE__);
    DL_APPEND(socket->msg_queue, message);

    return 0;
}

PILF_PRIVATE int _handle_pollout_event(minion_context_t * const ctx,
    minion_link_event_t * const event)
{
    minion_message_t * message;
    minion_socket_t * socket;
    char * buf = NULL;
    int bytes;
    int msg_len;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_EQ(MINION_LINK_POLLOUT_EVENT, event->type);
    ASSERT_TRUE(event->fd >= 0);
    ASSERT_NULL(event->msg);

    /* Search the fist message related with the given fd */
    DL_SEARCH_SCALAR(ctx->sockets, socket, fd, event->fd);
    CHECK_NNULL_R(socket, __LINE__);
    message = socket->msg_queue;
    DL_DELETE(socket->msg_queue, socket->msg_queue);

    /* Send */
    msg_len = strlen(message->msg) * sizeof(char);
    buf = nn_allocmsg(msg_len, 0);
    CHECK_NNULL_R(buf, __LINE__);
    strcpy(buf, message->msg);
    bytes = nn_send(event->fd, &buf, NN_MSG, NN_DONTWAIT);
    if (bytes != msg_len) {
        return __LINE__;
    }

    minion_secure_free(ctx, message->msg, strlen(message->msg));
    minion_secure_free(ctx, message, sizeof(minion_message_t));

    return 0;
}

PILF_INTERNAL int link_poll(minion_context_t * const ctx)
{
    struct nn_pollfd * polls; 
    int ret;
    int result;
    minion_socket_t * socket;
    unsigned i;
    size_t mem_amount, sockets_amount;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_TRUE(ctx->step_time > 0);

    /* Allocate enough nn_pollfd's*/
    DL_COUNT(ctx->sockets, socket, sockets_amount);
    mem_amount = sizeof(struct nn_pollfd) * sockets_amount;
    polls = (struct nn_pollfd *) minion_secure_malloc(ctx, mem_amount);
    CHECK_NNULL_R(polls, __LINE__);

    i = 0;
    DL_FOREACH(ctx->sockets, socket) {
        polls[i].fd = socket->fd;
        polls[i].events = NN_POLLIN;
        if(!DL_IS_EMPTY(socket->msg_queue)) {
            polls[i].events |= NN_POLLOUT;
        }
        i++;
    }

    ret = nn_poll(polls, sockets_amount, ctx->step_time);
    result = 0;

    /* Timeout */
    if (ret == 0) {
        result = 0;
        goto clean;
    }

    /* Error */
    if (ret == -1) {
        error(ctx, "%s", nn_strerror(nn_errno()));
        result = __LINE__;
        goto clean;
    }

    /* Handle data from sockets */
    for (i = 0; i < sockets_amount; ++i) {
        if (IS_BITS_IN(NN_POLLIN, polls[i].revents)) {
            EVENTS_NOTIFY_NEW_EVENT(link, ctx, MINION_LINK_POLLIN_EVENT, polls[i].fd, NULL, 0);
        }
        if (IS_BITS_IN(NN_POLLOUT, polls[i].revents)) {
            EVENTS_NOTIFY_NEW_EVENT(link, ctx, MINION_LINK_POLLOUT_EVENT, polls[i].fd, NULL, 0);
        }
    }

clean:
    minion_secure_free(ctx, polls, mem_amount);
    return result;
}

PILF_INTERNAL int link_add_socket(minion_context_t * const ctx, int fd, int eid)
{
    minion_socket_t * socket;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_TRUE(fd >= 0);
    ASSERT_TRUE(eid > 0);

    DL_NEW_NODE(ctx, socket, minion_socket_t)
    socket->fd = fd;
    socket->eid = eid;
    socket->msg_queue = NULL;
    DL_APPEND(ctx->sockets, socket);

    return 0;
}

PILF_INTERNAL minion_socket_t * link_get_master_socket(
    minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);

    /* The socket with the master is the first alwais */
    return ctx->sockets;
}

PILF_INTERNAL int link_init(minion_context_t * const ctx)
{
    int master_socket_fd;
    int master_socket_eid;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_STR(ctx->master_url);
    ASSERT_TRUE(events_ready(ctx));

    /* Init list of sockets */
    ctx->sockets = NULL;

    /* Add socket to Master */
    master_socket_fd = nn_socket(AF_SP, NN_REP);
    CHECK_TRUE_R(master_socket_fd >= 0, __LINE__);
    master_socket_eid = nn_connect(master_socket_fd, ctx->master_url);
    CHECK_TRUE_R(master_socket_eid > 0, __LINE__);
    link_add_socket(ctx, master_socket_fd, master_socket_eid);

    /* Bind */
    EVENTS_BIND_NEW_LISTENER(link, ctx, MINION_LINK_POLLIN_EVENT,
        _handle_pollin_event);
    EVENTS_BIND_NEW_LISTENER(link, ctx, MINION_LINK_OUTMSG_EVENT,
        _handle_outmsg_event);
    EVENTS_BIND_NEW_LISTENER(link, ctx, MINION_LINK_POLLOUT_EVENT,
        _handle_pollout_event);

    return 0;
}

PILF_INTERNAL int link_finalize(minion_context_t * const ctx)
{
    minion_socket_t * socket;
    minion_socket_t * tmp_socket;
    minion_message_t * message;
    minion_message_t * tmp_message;

    MINION_TRACE_FUNCTION();
    
    ASSERT_NNULL(ctx);

    DL_FOREACH_SAFE(ctx->sockets, socket, tmp_socket) {
        /* Shutdown connection */
        CHECK_ZERO_R(nn_shutdown(socket->fd, socket->eid), __LINE__);

        /* Clean message queue */
        DL_FOREACH_SAFE(socket->msg_queue, message, tmp_message) {
            /* Free message */
            minion_secure_free(ctx, message->msg, strlen(message->msg));
            /* Delete from list */
            DL_DELETE(socket->msg_queue, message);
            /* Free node */
            DL_FINALIZE_NODE(ctx, message, minion_message_t);
        }
        ASSERT_NULL(socket->msg_queue);

        /* Delete from list */
        DL_DELETE(ctx->sockets, socket);

        /* Free node */
        DL_FINALIZE_NODE(ctx, socket, minion_socket_t);
    }
    ASSERT_NULL(ctx->sockets);

    return 0;
}
