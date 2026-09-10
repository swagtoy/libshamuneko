#ifndef _SHAMUNEKO_H_
#define _SHAMUNEKO_H_

#include "shamuneko_module.h"

typedef struct _shamuneko_state shamuneko_state_t;
struct _shamuneko_state;

struct shamuneko_http_funcs
{
	void*  (*create_session)();
	int    (*request)(void *data, char *url);
	void   (*destroy_session)(void *data);
};
#define SHAMUNEKO_HTTP_NOOP ((struct shamuneko_http_funcs){ 0 })

shamuneko_state_t*  shamuneko_new(struct shamuneko_http_funcs funcs);
void                shamuneko_destroy(shamuneko_state_t *state);

#endif // _SHAMUNEKO_H_
