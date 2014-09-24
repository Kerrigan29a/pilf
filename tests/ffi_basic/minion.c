
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

const char  * exec_name;
const char  * master_url;
minion_context_t ctx;

void hidden_func()
{
    printf("hi from hidden func\n");
}

void stop_dispatcher(int signal)
{
    switch(signal){
    case SIGINT:
        warning(&ctx, "Quitting by SIGINT");
        break;
    case SIGKILL:
        warning(&ctx, "Quitting by SIGKILL");
        break;
    case SIGTERM:
        warning(&ctx, "Quitting by SIGTERM");
        break;
    default:
        critical(&ctx, "Unacheable section reached");
        abort();
    }

    ctx.terminate = true;
}

static void run(void **state)
{
    FILE *f;

    minion_context_init(&ctx);
    ctx.master_url =    (char *) master_url;

    f = fopen("address.txt", "w");
    assert_true(f != NULL);
    fprintf(f, "%p\n", hidden_func);
    fclose(f);

    signal(SIGINT,  stop_dispatcher);
    signal(SIGKILL, stop_dispatcher);
    signal(SIGTERM, stop_dispatcher);
    
    minion_start(&ctx);
    minion_stop(&ctx);

    remove("address.txt");
}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("%s <url>\n", argv[0]);
        return 1;
    }

    const UnitTest tests[] = {
        unit_test(run),
    };

    exec_name = argv[0];
    master_url = argv[1];

    return run_tests(tests, "ffi_basic");
}