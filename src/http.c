#include <stdlib.h>
#include <lua5.4/lauxlib.h>
#include "http.h"

static int
_luafunc_httpsession_request(lua_State *L)
{
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));
	void **data = luaL_checkudata(L, 1, "HTTPSession");
	shamuneko_request_t *req = calloc(1, sizeof(struct _shamuneko_request));

	req->st = st;
	req->callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	char const *url = lua_tostring(L, -1);

	DEBUGF("Request URL: %s", url);
	if (st->funcs.request) st->funcs.request(*data, url, req);

	return 1;
}

static int
_luafunc_httpsession_create(lua_State *L)
{
	void **data;
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));

	data = lua_newuserdatauv(_L, sizeof(void*), 0);
	// clear it incase st->funcs.create_session isn't set
	*data = NULL;

	luaL_getmetatable(_L, "HTTPSession");
	lua_setmetatable(_L, -2);

	if (st->funcs.create_session) *data = st->funcs.create_session();

	return 1;
}

static int
_luafunc_httpsesssion_destroy(lua_State *L)
{
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));
	void **data = luaL_checkudata(L, 1, "HTTPSession");

	if (st->funcs.destroy_session) st->funcs.destroy_session(*data);

	return 0;
}

static struct luaL_Reg _httpsession_meths[] = {
	{ "request", _luafunc_httpsession_request },
	{ "__gc",    _luafunc_httpsesssion_destroy },
	{ 0, 0 }
};

static struct luaL_Reg _httpsession_funcs[] = {
	{ "new", _luafunc_httpsession_create },
	{ 0, 0 }
};

void
create_httpsession_table(shamuneko_state_t *st)
{
	luaL_newmetatable(_L, "HTTPSession");

	lua_pushliteral(_L, "__index");
	lua_pushvalue(_L, -2);
	lua_rawset(_L, -3);

	// setup methods and funcs
	lua_pushlightuserdata(_L, st);
	luaL_setfuncs(_L, _httpsession_meths, 1);
	lua_pop(_L, 1);

	luaL_newlibtable(_L, _httpsession_funcs);
	lua_pushlightuserdata(_L, st);
	luaL_setfuncs(_L, _httpsession_funcs, 1);

	lua_setglobal(_L, "httpsession");

	_ASSERT_TOP;
}
