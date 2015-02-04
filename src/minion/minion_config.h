#ifndef __PILF_MINION__MINION_CONFIG_H__
#define __PILF_MINION__MINION_CONFIG_H__

/* Toggle the tracing of the functions of the lua engine */
#define MINION_TRACE_ENGINE                 false

/* Toggle the tracing of the functions of the event manager */
#define MINION_TRACE_EVENTS                 false

/* Toggle the tracing of the functions of the memory manager */
#define MINION_TRACE_MEMORY                 false

/* Toggle the tracing of the functions of the communications module */
#define MINION_TRACE_LINK                   false

/* Toggle the tracing of the functions that connect the communication module
 with the lua engine and with the event handler */
#define MINION_TRACE_MESSAGE_WORKFLOW       false

/* Toggle the tracing of the functions of the main functions (start and stop) */
#define MINION_TRACE_MINION                 false


/* Enable alias "=" with "return " like in the Lua interpreter */
#define MINION_ENABLE_SHOW_VALUE_WITH_EQUAL false

/* Enable colorized console */
#define MINION_ENABLE_CONSOLE_COLORS        true


#endif /* __PILF_MINION__MINION_CONFIG_H__ */