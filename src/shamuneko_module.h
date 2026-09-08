#ifndef _SHAMUNEKO_MODULE_H_
#define _SHAMUNEKO_MODULE_H_

typedef struct _shamuneko_state shamuneko_state_t;
struct _shamuneko_state;

typedef struct _shamuneko_module shamuneko_module_t;
struct _shamuneko_module;

shamuneko_module_t* shamuneko_module_load(shamuneko_state_t *state, const char *filename);
void	            shamuneko_module_destroy(shamuneko_module_t *module);

void                shamuneko_module_search(shamuneko_module_t *module, char *query);

#endif // _SHAMUNEKO_MODULE_H_
