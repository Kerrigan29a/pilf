#include <stdarg.h>
#include <stdio.h>

#include "minion_internals.h"

#if defined(__GNUC__) || defined(__clang__) || defined(__llvm__)
/* support printf format checking (3=the format string, 4=start of varargs) */
void log_log(minion_context_t * const ctx, const char * const prefix,
    const char * format, ...) __attribute__ (( format( printf, 3, 4) ));
#endif

#if MINION_ENABLE_CONSOLE_COLORS
    #define FPRINTF(color, fd, ...) cc_fprintf(color, fd, __VA_ARGS__)
    #define PREFIX_COLOR(prefix) (\
        IS_STREQ(prefix, "T")? CC_FG_DARK_CYAN : (\
        IS_STREQ(prefix, "D")? CC_FG_BLUE : (\
        IS_STREQ(prefix, "I")? CC_FG_GREEN : (\
        IS_STREQ(prefix, "W")? CC_FG_YELLOW : (\
        IS_STREQ(prefix, "E")? CC_FG_RED : (\
        /* IS_STREQ(prefix, "C") */ CC_FG_MAGENTA ))))))
#else
    #define FPRINTF(color, fd, format, ...) fprintf(fd, format, __VA_ARGS__)
    #define PREFIX_COLOR(prefix) CC_FG_NONE
#endif


PILF_INTERNAL void log_log(minion_context_t * const ctx, const char * const prefix,
    const char * format, ...)
{
    va_list args;

    ASSERT_NNULL(ctx);
    ASSERT_STR(prefix);
    ASSERT_STR(format);

    va_start(args, format);
    if (IS_NNULL(ctx->log_prefix)) {
        FPRINTF(CC_FG_WHITE, ctx->log_file, "[%s]", ctx->log_prefix);
        FPRINTF(CC_FG_NONE, ctx->log_file, " ");
    }
    FPRINTF(PREFIX_COLOR(prefix), ctx->log_file, "(%s):", prefix);
    FPRINTF(CC_FG_NONE, ctx->log_file, " ");
    vfprintf(ctx->log_file, format, args);
    FPRINTF(CC_FG_NONE, ctx->log_file, "\n");
    va_end(args);
}