#include <stdlib.h>
#include "shamuneko.h"
#include "http.h"

#include "shamuneko_private.h"

static void
_push_http_funcs(shamuneko_state_t *st)
{
	lua_pushlightuserdata(_L, st);
	lua_pushcclosure(_L, luafunc_create_httpsession, 1);
	lua_setglobal(_L, "httpsession");
}

shamuneko_state_t*
shamuneko_new(struct shamuneko_http_funcs funcs)
{
	shamuneko_state_t *st = calloc(1, sizeof(shamuneko_state_t));
	if (!st)
		return NULL;
	st->funcs = funcs;

	_L = luaL_newstate();

	_push_http_funcs(st);
	luaL_openlibs(_L);

	return st;
}

void
shamuneko_destroy(shamuneko_state_t *st)
{
	lua_close(_L);
}
