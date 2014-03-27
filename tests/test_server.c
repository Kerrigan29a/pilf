
#define UNIT_TESTING 1

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <minion_internals.h>

#include <cmocka.h>

/* A test case that does nothing and succeeds. */
static void test__parse_addr(void **state) {
    const char *host =  NULL;
    size_t host_len =   0;
    const char *port =  NULL;
    size_t port_len =   0;
    int r = 0;

    /* Test incorrect `addr` */
    expect_assert_failure(_parse_addr(NULL, &host, &host_len, &port, &port_len));
    /* Test incorrect `host` */
    expect_assert_failure(_parse_addr("tcp://localhost:italk", NULL, &host_len, &port, &port_len));
    /* Test incorrect `host_len` */
    expect_assert_failure(_parse_addr("tcp://localhost:italk", &host, NULL, &port, &port_len));
    /* Test incorrect `port` */
    expect_assert_failure(_parse_addr("tcp://localhost:italk", &host, &host_len, NULL, &port_len));
    /* Test incorrect `port_len` */
    expect_assert_failure(_parse_addr("tcp://localhost:italk", &host, &host_len, &port, NULL));

    /* Test a valid TCP address with port */
    r = _parse_addr("tcp://localhost:italk", &host, &host_len, &port, &port_len);
    assert_int_equal(r, TCP_ADDR);
    assert_string_equal(host, "localhost:italk");
    assert_int_equal(host_len, 9);
    assert_string_equal(port, "italk");
    assert_int_equal(port_len, 5);
    /* Test a valid TCP address without port */
    r = _parse_addr("tcp://localhost", &host, &host_len, &port, &port_len);
    assert_int_equal(r, TCP_ADDR);
    assert_string_equal(host, "localhost");
    assert_int_equal(host_len, 9);
    assert_null(port);
    assert_int_equal(port_len, 0);
    /* Test a valid TCP address with port */
    r = _parse_addr("tcp://:italk", &host, &host_len, &port, &port_len);
    assert_int_equal(r, TCP_ADDR);
    assert_string_equal(host, ":italk");
    assert_int_equal(host_len, 0);
    assert_string_equal(port, "italk");
    assert_int_equal(port_len, 5);
    /* Test an invalid TCP address with port */
    r = _parse_addr("tcp:/localhost:italk", &host, &host_len, &port, &port_len);
    assert_int_equal(r, -1);

    /* Test a valid IPC address with port */
    r = _parse_addr("ipc:/tmp/uv-test-sock", &host, &host_len, &port, &port_len);
    assert_int_equal(r, PIPE_ADDR);
    assert_string_equal(host, "/tmp/uv-test-sock");
    assert_int_equal(host_len, 17);
    assert_null(port);
    assert_int_equal(port_len, 0);
    /* Test a invalid IPC address with port */
    r = _parse_addr("ipc:/tmp/uv-test-sock:1234", &host, &host_len, &port, &port_len);
    assert_int_equal(r, PIPE_ADDR);
    assert_string_equal(host, "/tmp/uv-test-sock:1234");
    assert_int_equal(host_len, 22);
    assert_null(port);
    assert_int_equal(port_len, 0);
    /* Test a invalid IPC address with port */
    r = _parse_addr("ipc::1234", &host, &host_len, &port, &port_len);
    assert_int_equal(r, PIPE_ADDR);
    assert_string_equal(host, ":1234");
    assert_int_equal(host_len, 5);
    assert_null(port);
    assert_int_equal(port_len, 0);
    /* Test a valid IPC address */
    r = _parse_addr("ipc:tmp/uv-test-sock", &host, &host_len, &port, &port_len);
    assert_int_equal(r, PIPE_ADDR);
    assert_string_equal(host, "tmp/uv-test-sock");
    assert_int_equal(host_len, 16);
    assert_null(port);
    assert_int_equal(port_len, 0);

    /* Test a valid UDP address with port */
    r = _parse_addr("udp://192.168.0.100:12345", &host, &host_len, &port, &port_len);
    assert_int_equal(r, -1);
}

int main(void) {
    const UnitTest tests[] = {
        unit_test(test__parse_addr),
    };

    return run_tests(tests);
}