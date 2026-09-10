#ifndef _SHAMUNEKO_H_
#define _SHAMUNEKO_H_

#include <stddef.h>
#include "shamuneko_module.h"

typedef struct _shamuneko_state shamuneko_state_t;
struct _shamuneko_state;

typedef struct _shamuneko_request shamuneko_request_t;
struct _shamuneko_request;

struct shamuneko_http_funcs
{
	void*  (*create_session)();
	int    (*request)(void *data, char const *url, shamuneko_request_t *shamuneko_internal);
	void   (*destroy_session)(void *data);
};
#define SHAMUNEKO_HTTP_NOOP ((struct shamuneko_http_funcs){ 0 })

shamuneko_state_t*  shamuneko_new(struct shamuneko_http_funcs funcs);
void                shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t len);
void                shamuneko_destroy(shamuneko_state_t *state);

#endif // _SHAMUNEKO_H_
