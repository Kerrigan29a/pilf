#ifndef __PILF_MINION__LOG_H__
#define __PILF_MINION__LOG_H__


#define trace(ctx, ...)     log_log(ctx, "T", __VA_ARGS__)
#define debug(ctx, ...)     log_log(ctx, "D", __VA_ARGS__)
#define info(ctx, ...)      log_log(ctx, "I", __VA_ARGS__)
#define warning(ctx, ...)   log_log(ctx, "W", __VA_ARGS__)
#define error(ctx, ...)     log_log(ctx, "E", __VA_ARGS__)
#define critical(ctx, ...)  log_log(ctx, "C", __VA_ARGS__)


#endif /* __PILF_MINION__LOG_H__ */