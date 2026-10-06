#ifndef _SHAMUNEKO_H_
#define _SHAMUNEKO_H_

#include <stdbool.h>
#include <stddef.h>
#include "shamuneko_types.h"
#include "shamuneko_module.h"

shamuneko_state_t*  shamuneko_new(shamuneko_flags_t flags, struct shamuneko_http_funcs funcs);
void                shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t len);
void                shamuneko_download_pages(shamuneko_state_t *state, struct shamuneko_pages_result *pages, size_t len, download_pages_callback_t cb, void *data);
void                shamuneko_destroy(shamuneko_state_t *state);

#endif // _SHAMUNEKO_H_
