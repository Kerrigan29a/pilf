
#define UNIT_TESTING 1

/* Import before cmockery */
#include <minion_internals.h>
#include <string.h>
#include <limits.h>
#include <libgen.h>
#include <signal.h>
/* cmockery deps */
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery/cmockery.h>

#define TEST_NAME           "shared_tcp_basic"
#define MINION_MASTER_URL   "tcp://127.0.0.1:12345"

minion_context_t ctx;

static void run(void **state)
{
    minion_context_init(&ctx);
    ctx.master_url =    (char *) MINION_MASTER_URL;
    ctx.log_prefix =    TEST_NAME;
    
    minion_start(&ctx);
}

void __attribute__ ((constructor)) _start(void);
void __attribute__ ((destructor)) _stop(void);

void _start()
{
    const UnitTest tests[] = {
        unit_test(run),
    };

    run_tests(tests, TEST_NAME);
}

void _stop()
{
    minion_stop(&ctx);
}