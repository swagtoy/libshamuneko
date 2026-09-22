#include <stdlib.h>
#include <lua5.4/lauxlib.h>
#include "http.h"
#include "util.h"

#include "shamuneko_private.h"

#define _METATABLE_NAME "HTTPSession"

static int
_luafunc_httpsession_request(lua_State *L)
{
	// TODO move below to ext func
	struct _result_data *result = data_from_regidx(L);

	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));
	shamuneko_request_t *req = calloc(1, sizeof(struct _shamuneko_request));
	void **data = luaL_checkudata(L, 1, _METATABLE_NAME);
	char const *url = lua_tostring(L, -1);

	req->co = L;
	req->st = st;

	DEBUGF("Request URL: %s", url);

	if (st->funcs.request) st->funcs.request(*data, url, req);

	if (ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
		// TODO: verify these arguments are on the stack
		// the request was directly performed in the request func
		// above. thus, we return 1 argument since
		// shamuneko_process_request _should have_ already pushed our arguments
		return 1;
	else
	{
		// so it doesn't get garbage collected
		lua_pushthread(L);
		req->thread_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		return lua_yield(L, 0);
	}
}

static int
_luafunc_httpsession_create(lua_State *L)
{
	void **data;
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));

	data = lua_newuserdatauv(_L, sizeof(void*), 0);
	// clear it incase st->funcs.create_session isn't set
	*data = NULL;

	luaL_getmetatable(_L, _METATABLE_NAME);
	lua_setmetatable(_L, -2);

	if (st->funcs.create_session) *data = st->funcs.create_session();

	return 1;
}

static int
_luafunc_httpsession_destroy(lua_State *L)
{
	shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));
	void **data = luaL_checkudata(L, 1, _METATABLE_NAME);

	if (st->funcs.destroy_session) st->funcs.destroy_session(*data);

	return 0;
}

static struct luaL_Reg _httpsession_meths[] = {
	{ "request", _luafunc_httpsession_request },
	{ "__gc",    _luafunc_httpsession_destroy },
	{ 0, 0 }
};

static struct luaL_Reg _httpsession_funcs[] = {
	{ "new", _luafunc_httpsession_create },
	{ 0, 0 }
};

void
create_httpsession_table(shamuneko_state_t *st)
{
	luaL_newmetatable(_L, _METATABLE_NAME);

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
