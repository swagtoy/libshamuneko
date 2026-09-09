#include "http.h"

static int
_luafunc_http_request(lua_State *L)
{
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));

	lua_pushstring(L, "kerfluffle");
	lua_pcall(L, 1, LUA_MULTRET, 0);
	char *url = lua_tostring(L, -1);

	DEBUGF("Request URL: %s", url);
	if (st->funcs.request) st->funcs.request(url);

	return 1;
}

void
create_http_table(shamuneko_state_t *st,
                  lua_State *L)
{
	lua_newtable(L);
	lua_pushstring(L, "request");
	lua_pushlightuserdata(_L, st);
	lua_pushcclosure(L, _luafunc_http_request, 1);
	lua_settable(L, -3);
}

int
luafunc_create_httpsession(lua_State *L)
{
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));

	if (st->funcs.create_session) st->funcs.create_session();

	create_http_table(st, L);

	return 1;
}
