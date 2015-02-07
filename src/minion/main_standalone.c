#include <string.h>
#include <libgen.h>
#include <signal.h>

#include "minion.h"
#include "main_config.h"
#include "../common/check.h"

minion_context_t ctx;

#define PATH_MAX 4096

void stop_dispatcher(int signal)
{
    switch(signal){
    case SIGINT:
        printf("Quitting by SIGINT");
        break;
    case SIGKILL:
        printf("Quitting by SIGKILL");
        break;
    case SIGTERM:
        printf("Quitting by SIGTERM");
        break;
    default:
        printf("Unacheable section reached");
        abort();
}
    ctx.terminate = true;
}

int main(int argc, char const *argv[])
{
    char absolute_path[PATH_MAX];

    /* XXX: Total crap */
    {
        if (strlcpy(absolute_path, dirname((char *) argv[0]), sizeof(absolute_path)) >= sizeof(absolute_path)) {
            perror("argv[0] is too long");
            return 1;
        }
        if (strlcat(absolute_path, "/", sizeof(absolute_path)) >= sizeof(absolute_path)) {
            perror("argv[0] + \"/\" is too long");
            return 1;
        }
        if (strlcat(absolute_path, MINION_BOOT_FILE_NAME, sizeof(absolute_path)) >= sizeof(absolute_path)) {
            perror("argv[0] + \"/\" + MINION_BOOT_FILE_NAME is too long");
            return 1;
        }
    }

    minion_context_init(&ctx);
    ctx.file_name =     absolute_path;
    ctx.master_url =    MINION_MASTER_URL;
    ctx.log_prefix =    "minion-standalone";
    
    signal(SIGINT,  stop_dispatcher);
    signal(SIGKILL, stop_dispatcher);
    signal(SIGTERM, stop_dispatcher);

    minion_start(&ctx);
    minion_stop(&ctx);

    return 0;
}
