#ifndef _SHAMUNEKO_MODULE_H_
#define _SHAMUNEKO_MODULE_H_

#include "shamuneko_types.h"

shamuneko_module_t* shamuneko_module_load(shamuneko_state_t *state, const char *filename);
void	            shamuneko_module_destroy(shamuneko_module_t *module);

void                shamuneko_module_search(shamuneko_module_t *module, char *query);
void                shamuneko_module_get_trending(shamuneko_module_t *module,
                                                  get_trending_callback_t cb,
                                                  void *data);
void                shamuneko_module_get_pages(shamuneko_module_t *module,
                                               char const *id,
                                               unsigned chapter,
                                               get_pages_callback_t cb,
                                               void *data);

#endif // _SHAMUNEKO_MODULE_H_
