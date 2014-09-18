#include <stdarg.h>
#include <stdio.h>

#include "minion_internals.h"

#if defined(__GNUC__) || defined(__clang__) || defined(__llvm__)
/* support printf format checking (3=the format string, 4=start of varargs) */
PILF_INTERNAL void log_log(minion_context_t * const ctx, const char * const preffix,
    const char * format, ...) __attribute__ (( format( printf, 3, 4) ));
#endif

PILF_INTERNAL void log_log(minion_context_t * const ctx, const char * const preffix,
    const char * format, ...)
{
    va_list args;

    ASSERT_NNULL(ctx);
    ASSERT_STR(preffix);
    ASSERT_STR(format);

    va_start(args, format);
    if (IS_NNULL(ctx->log_prefix)) {
        fprintf(ctx->log_file, "[%s] ", ctx->log_prefix);
    }
    fprintf(ctx->log_file, "(%s): ", preffix);
    vfprintf(ctx->log_file, format, args);
    fprintf(ctx->log_file, "\n");
    va_end(args);
}