#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <stdio.h>

#include "minion_internals.h"

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

static int server_closed;
static addr_type serverType;
static uv_tcp_t tcpServer;
static uv_udp_t udpServer;
static uv_pipe_t pipeServer;
static uv_handle_t * server;

PILF_PRIVATE void on_close(uv_handle_t* peer)
{
    free(peer);
}

PILF_PRIVATE void on_server_close(uv_handle_t* handle)
{
}

PILF_PRIVATE void after_write(uv_write_t* req, int status)
{
    write_req_t* wr;

    /* Free the read/write buffer and the request */
    wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);

    if (status == 0) {
        return;
    }

    fprintf(stderr, "uv_write error: %s\n", uv_strerror(status));

    if (status == UV_ECANCELED) {
        return;
    }

    uv_close((uv_handle_t*)req->handle, on_close);
}


PILF_PRIVATE void after_shutdown(uv_shutdown_t* req, int status)
{
  uv_close((uv_handle_t*)req->handle, on_close);
  free(req);
}


PILF_PRIVATE void after_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
{
    int i;
    write_req_t *wr;
    uv_shutdown_t* req;

    if (nread < 0) {
        /* Error or EOF */

        if (buf->base) {
            free(buf->base);
        }

        req = (uv_shutdown_t*) malloc(sizeof *req);
        uv_shutdown(req, handle, after_shutdown);

        return;
    }

    if (nread == 0) {
        /* Everything OK, but nothing read. */
        free(buf->base);
        return;
    }

    /*
    * Scan for the letter Q which signals that we should quit the server.
    * If we get QS it means close the stream.
    */
    if (!server_closed) {
        for (i = 0; i < nread; i++) {
            if (buf->base[i] == 'Q') {
                if (i + 1 < nread && buf->base[i + 1] == 'S') {
                    free(buf->base);
                    uv_close((uv_handle_t*)handle, on_close);
                    return;
                } else {
                    uv_close(server, on_server_close);
                    server_closed = 1;
                }
            }
        }
    }

    wr = (write_req_t*) malloc(sizeof *wr);

    wr->buf = uv_buf_init(buf->base, nread);
    if (uv_write(&wr->req, handle, &wr->buf, 1, after_write)) {
        fprintf(stderr,"uv_write failed");
    }
}

PILF_PRIVATE void echo_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}


PILF_PRIVATE void on_connection(uv_stream_t* server, int status) {
    uv_stream_t* stream;
    int r;

    if (status != 0) {
        fprintf(stderr, "Connect error %s\n", uv_err_name(status));
    }

    switch (serverType) {
        case TCP_ADDR:
            stream = malloc(sizeof(uv_tcp_t));
            r = uv_tcp_init(uv_default_loop(), (uv_tcp_t*)stream);
            break;

        case PIPE_ADDR:
            stream = malloc(sizeof(uv_pipe_t));
            r = uv_pipe_init(uv_default_loop(), (uv_pipe_t*)stream, 0);
            break;
        default:
            // TODO: caboom
            break;
    }

    /* associate server with stream */
    stream->data = server;

    r = uv_accept(server, stream);

    r = uv_read_start(stream, echo_alloc, after_read);
}

PILF_PRIVATE void on_recv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* rcvbuf,
    const struct sockaddr* addr, unsigned flags)
{
  uv_udp_send_t* req;
  uv_buf_t sndbuf;

  req = malloc(sizeof(*req));

  sndbuf = *rcvbuf;
}


PILF_PRIVATE void on_send(uv_udp_send_t* req, int status)
{
    free(req);
}


PILF_PRIVATE int tcp4_echo_start(int port)
{
    struct sockaddr_in addr;
    int r;

    uv_ip4_addr("0.0.0.0", port, &addr);

    server = (uv_handle_t*)&tcpServer;
    serverType = TCP_ADDR;

    r = uv_tcp_init(uv_default_loop(), &tcpServer);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Socket creation error\n");
        return 1;
    }

    r = uv_tcp_bind(&tcpServer, (const struct sockaddr*) &addr, 0);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Bind error\n");
        return 1;
    }

    r = uv_listen((uv_stream_t*)&tcpServer, SOMAXCONN, on_connection);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Listen error %s\n", uv_err_name(r));
        return 1;
    }

    return 0;
}


PILF_PRIVATE int tcp6_echo_start(int port)
{
    struct sockaddr_in6 addr6;
    int r;

    uv_ip6_addr("::1/128", port, &addr6);

    server = (uv_handle_t*)&tcpServer;
    serverType = TCP_ADDR;

    r = uv_tcp_init(uv_default_loop(), &tcpServer);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Socket creation error\n");
        return 1;
    }

    /* IPv6 is optional as not all platforms support it */
    r = uv_tcp_bind(&tcpServer, (const struct sockaddr*) &addr6, 0);
    if (r) {
        /* show message but return OK */
        fprintf(stderr, "IPv6 not supported\n");
        return 0;
    }

    r = uv_listen((uv_stream_t*)&tcpServer, SOMAXCONN, on_connection);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Listen error\n");
        return 1;
    }

    return 0;
}


PILF_PRIVATE int udp4_echo_start(int port)
{
    int r;

    server = (uv_handle_t*)&udpServer;
    serverType = UDP_ADDR;

    r = uv_udp_init(uv_default_loop(), &udpServer);
    if (r) {
        fprintf(stderr, "uv_udp_init: %s\n", uv_strerror(r));
        return 1;
    }

    r = uv_udp_recv_start(&udpServer, echo_alloc, on_recv);
    if (r) {
        fprintf(stderr, "uv_udp_recv_start: %s\n", uv_strerror(r));
        return 1;
    }

    return 0;
}


PILF_PRIVATE int pipe_echo_start(char* pipeName)
{
    int r;

    #ifndef _WIN32
    {
        uv_fs_t req;
        uv_fs_unlink(uv_default_loop(), &req, pipeName, NULL);
        uv_fs_req_cleanup(&req);
    }
    #endif

    server = (uv_handle_t*)&pipeServer;
    serverType = PIPE_ADDR;

    r = uv_pipe_init(uv_default_loop(), &pipeServer, 0);
    if (r) {
        fprintf(stderr, "uv_pipe_init: %s\n", uv_strerror(r));
        return 1;
    }

    r = uv_pipe_bind(&pipeServer, pipeName);
    if (r) {
        fprintf(stderr, "uv_pipe_bind: %s\n", uv_strerror(r));
        return 1;
    }

    r = uv_listen((uv_stream_t*)&pipeServer, SOMAXCONN, on_connection);
    if (r) {
        fprintf(stderr, "uv_pipe_listen: %s\n", uv_strerror(r));
        return 1;
    }

    return 0;
}


PILF_PRIVATE int _parse_addr(const char *addr, const char **host,
    size_t *host_len, const char **port, size_t *port_len)
{
    /*
     * char * addr = "tcp://localhost:italk";
     * char * addr = "ipc:/tmp/uv-test-sock";
     */

    const char *delim1;
    const char *delim2;

    ASSERT_STR(addr);
    ASSERT_NOT_NULL(host);
    ASSERT_NOT_NULL(host_len);
    ASSERT_NOT_NULL(port);
    ASSERT_NOT_NULL(port_len);

    *host = NULL;
    *host_len = 0;
    *port = NULL;
    *port_len = 0;
    
    /* Get host delim */
    delim1 = strchr(addr, ':');
    if (delim1 == NULL) {
        return -1;
    }

    /* Check IPC */
    if (strncmp(addr, "ipc:", 4) == 0) {
        *host = delim1 + 1;
        *host_len = (addr + strlen(addr)) - *host;
        return PIPE_ADDR;
    }

    /* If not IPC, search for port delim */
    delim2 = strchr(delim1 + 1, ':');
    if (delim2 != NULL) {
        *port = delim2 + 1;
        *port_len = (addr + strlen(addr)) - *port;
    } else {
        delim2 = (addr + strlen(addr));
    }

    /* Check TCP */
    if (strncmp(addr, "tcp://", 6) == 0) {
        *host = delim1 + 3;
        *host_len = delim2 - (delim1 + 3);
        return TCP_ADDR;
    }

    return -1;
}


PILF_INTERNAL int server_run(const char *addr)
{
    const char *host = NULL;
    const char *port = NULL;
    size_t host_len = 0;
    size_t port_len = 0;

    CHECK_NOT_NULL_RETURN(addr, 1);

    switch (_parse_addr(addr, &host, &host_len, &port, &port_len)) {
        case TCP_ADDR:
            printf("TCP:\n");
            printf("    host: %s\n", host);
            printf("    service: %s\n", port);
            break;
        case UDP_ADDR:
            printf("UDP\n");
            CHECK_UNREACHABLE_ABORT();
            break;
        case PIPE_ADDR:
            printf("PIPE\n");
            break;
        default:
            printf("DEFAULT\n");
            CHECK_UNREACHABLE_ABORT();
            break;
    }

    //tcp4_echo_start(12345);
    //pipe_echo_start(/tmp/uv-test-sock)
    //tcp6_echo_start(12345)
    //udp4_echo_start(12345)

    //uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}