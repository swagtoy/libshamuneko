#include <stdlib.h>
#include "shamuneko.h"
#include "http.h"

#include "shamuneko_private.h"

void
shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t data_len)
{
	struct _shamuneko_request *req = internal;
	shamuneko_state_t *st = req->st;
	lua_rawgeti(_L, LUA_REGISTRYINDEX, req->callback_ref);
	lua_pushlstring(_L, data, data_len);
	lua_pcall(_L, 1, LUA_MULTRET, 0);
}

shamuneko_state_t*
shamuneko_new(struct shamuneko_http_funcs funcs)
{
	shamuneko_state_t *st = calloc(1, sizeof(shamuneko_state_t));
	if (!st)
		return NULL;
	st->funcs = funcs;

	_L = luaL_newstate();

	create_httpsession_table(st);

	luaL_openlibs(_L);

	return st;
}

void
shamuneko_destroy(shamuneko_state_t *st)
{
	lua_close(_L);
}
