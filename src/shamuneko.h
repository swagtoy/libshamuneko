#ifndef _SHAMUNEKO_H_
#define _SHAMUNEKO_H_

#include <stddef.h>
#include "shamuneko_types.h"
#include "shamuneko_module.h"

shamuneko_state_t*  shamuneko_new(struct shamuneko_http_funcs funcs);
void                shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t len);
void                shamuneko_destroy(shamuneko_state_t *state);

#endif // _SHAMUNEKO_H_
