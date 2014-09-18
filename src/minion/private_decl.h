/*
 * 18/09/2014 13:53:13
 *
 * DO NOT EDIT!!!
 * This file was created automatically by:
 *
 * /Users/kerrigan29a/Dropbox/pilf//tools/declarations.py /Users/kerrigan29a/Dropbox/pilf/src//minion//engine.c /Users/kerrigan29a/Dropbox/pilf/src//minion//events.c /Users/kerrigan29a/Dropbox/pilf/src//minion//link.c /Users/kerrigan29a/Dropbox/pilf/src//minion//log.c /Users/kerrigan29a/Dropbox/pilf/src//minion//memory.c /Users/kerrigan29a/Dropbox/pilf/src//minion//message_workflow.c /Users/kerrigan29a/Dropbox/pilf/src//minion//minion.c -p PILF_PRIVATE -o /Users/kerrigan29a/Dropbox/pilf/src//minion//private_decl.h
 */

#ifndef __823980AB3256BB46AAFE390B20D466346692B850__
#define __823980AB3256BB46AAFE390B20D466346692B850__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PILF_PRIVATE
#define PILF_PRIVATE extern
#endif

/* engine.c */
PILF_PRIVATE int _handle_exec_event(minion_context_t * const ctx, minion_engine_event_t * const event);
/* events.c */
PILF_PRIVATE minion_listener_t ** _select_list(minion_context_t * const ctx, minion_event_type_e event_type);
PILF_PRIVATE int _handle_event(minion_context_t * const ctx, minion_basic_event_t * event);
PILF_PRIVATE void _delete_event(minion_context_t * const ctx, minion_basic_event_t ** event);
/* link.c */
PILF_PRIVATE int _handle_pollin_event(minion_context_t * const ctx, minion_link_event_t * const event);
PILF_PRIVATE int _handle_outmsg_event(minion_context_t * const ctx, minion_link_event_t * const event);
PILF_PRIVATE int _handle_pollout_event(minion_context_t * const ctx, minion_link_event_t * const event);
/* message_workflow.c */
PILF_PRIVATE int _handle_time_step_event(minion_context_t * const ctx, minion_time_event_t * const event);
PILF_PRIVATE int _handle_link_inmsg_event(minion_context_t * const ctx, minion_link_event_t * const event);
PILF_PRIVATE int _handle_engine_done_event(minion_context_t * const ctx, minion_engine_event_t * const event);

#ifdef __cplusplus
}
#endif

#endif /* __823980AB3256BB46AAFE390B20D466346692B850__ */
