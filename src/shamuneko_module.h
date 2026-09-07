#ifndef _SHAMUNEKO_MODULE_H_
#define _SHAMUNEKO_MODULE_H_

typedef struct _shamuneko_state shamuneko_state_t;
struct _shamuneko_state;

typedef struct _shamuneko_module shamuneko_module_t;
struct _shamuneko_module;

shamuneko_module_t* shamuneko_load_module(shamuneko_state_t *state, const char *filename);

#endif // _SHAMUNEKO_MODULE_H_
