#include <stdlib.h>
#include "shamuneko.h"

#include "shamuneko_private.h"

int
_httpsessioncaller(lua_State *L)
{
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));

	if (st->funcs.create_session) st->funcs.create_session();

	lua_newtable(L);
	return 1;
}

static void
_push_http_funcs(shamuneko_state_t *st)
{
	lua_pushlightuserdata(_L, st);
	lua_pushcclosure(_L, _httpsessioncaller, 1);
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
