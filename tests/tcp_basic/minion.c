
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

#define BOOT_FILE_NAME   "boot.lua"

const char  * exec_name;
const char  * master_url;
minion_context_t ctx;

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
    char absolute_path[PATH_MAX];

    assert_true(strlcpy(absolute_path, dirname((char *) exec_name), sizeof(absolute_path)) < sizeof(absolute_path));
    assert_true(strlcat(absolute_path, "/", sizeof(absolute_path)) < sizeof(absolute_path));
    assert_true(strlcat(absolute_path, BOOT_FILE_NAME, sizeof(absolute_path)) < sizeof(absolute_path));

    minion_context_init(&ctx);
    ctx.file_name =     absolute_path;
    ctx.script =        NULL;
    ctx.master_url =    (char *) master_url;

    signal(SIGINT,  stop_dispatcher);
    signal(SIGKILL, stop_dispatcher);
    signal(SIGTERM, stop_dispatcher);
    
    minion_start(&ctx);
    minion_stop(&ctx);
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

    return run_tests(tests, "tcp_basic");
}