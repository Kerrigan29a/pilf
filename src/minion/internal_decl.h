/*
 * 18/09/2014 13:53:13
 *
 * DO NOT EDIT!!!
 * This file was created automatically by:
 *
 * /Users/kerrigan29a/Dropbox/pilf//tools/declarations.py /Users/kerrigan29a/Dropbox/pilf/src//minion//engine.c /Users/kerrigan29a/Dropbox/pilf/src//minion//events.c /Users/kerrigan29a/Dropbox/pilf/src//minion//link.c /Users/kerrigan29a/Dropbox/pilf/src//minion//log.c /Users/kerrigan29a/Dropbox/pilf/src//minion//memory.c /Users/kerrigan29a/Dropbox/pilf/src//minion//message_workflow.c /Users/kerrigan29a/Dropbox/pilf/src//minion//minion.c -p PILF_INTERNAL -o /Users/kerrigan29a/Dropbox/pilf/src//minion//internal_decl.h
 */

#ifndef __AE4FAD2BFC9356F2405DCF23194759EA873B6D9F__
#define __AE4FAD2BFC9356F2405DCF23194759EA873B6D9F__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PILF_INTERNAL
#define PILF_INTERNAL extern
#endif

/* engine.c */
PILF_INTERNAL int engine_init(minion_context_t * const ctx);
PILF_INTERNAL int engine_finalize(minion_context_t * const ctx);
/* events.c */
PILF_INTERNAL void events_bind(minion_context_t * const ctx, minion_listener_t * const listener);
PILF_INTERNAL int events_notify(minion_context_t * const ctx, minion_basic_event_t * event);
PILF_INTERNAL int events_dispatch(minion_context_t * const ctx);
PILF_INTERNAL bool events_ready(minion_context_t * const ctx);
PILF_INTERNAL int events_init(minion_context_t * const ctx);
PILF_INTERNAL int events_finalize(minion_context_t * const ctx);
/* link.c */
PILF_INTERNAL int link_poll(minion_context_t * const ctx);
PILF_INTERNAL int link_add_socket(minion_context_t * const ctx, int fd, int eid);
PILF_INTERNAL minion_socket_t * link_get_master_socket( minion_context_t * const ctx);
PILF_INTERNAL int link_init(minion_context_t * const ctx);
PILF_INTERNAL int link_finalize(minion_context_t * const ctx);
/* log.c */
PILF_INTERNAL void log_log(minion_context_t * const ctx, const char * const preffix, const char * format, ...);
/* memory.c */
PILF_INTERNAL void * minion_memset(minion_context_t * const ctx, void *ptr, int c, size_t size);
PILF_INTERNAL void * _minion_malloc(minion_context_t * const ctx, size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION);
PILF_INTERNAL void * _minion_secure_malloc(minion_context_t * const ctx, size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION);
PILF_INTERNAL void _minion_free(minion_context_t * const ctx, void * ptr MEMORY_DEBUG_INFO_ARGS_DECLARATION);
PILF_INTERNAL void _minion_secure_free(minion_context_t * const ctx, void * ptr, size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION);
PILF_INTERNAL char * _minion_strndup(minion_context_t * const ctx, const char * str, size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION);
PILF_INTERNAL char * _minion_strdup(minion_context_t * const ctx, const char * str MEMORY_DEBUG_INFO_ARGS_DECLARATION);
/* message_workflow.c */
PILF_INTERNAL int message_workflow_init(minion_context_t * const ctx);
PILF_INTERNAL int message_workflow_finalize(minion_context_t * const ctx);

#ifdef __cplusplus
}
#endif

#endif /* __AE4FAD2BFC9356F2405DCF23194759EA873B6D9F__ */
