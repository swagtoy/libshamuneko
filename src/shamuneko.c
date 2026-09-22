#include <stdlib.h>
#include "shamuneko.h"
#include "http.h"
#include "html.h"
#include "json.h"
#include "returnfuncs.h"

#include "shamuneko_private.h"

#define SET_LFIELD(result, field) \
	lua_getfield(L, -1, #field); \
	if (lua_isstring(L, -1)) result->field = lua_tostring(L, -1); \
	lua_pop(L, 1)

// !!! TODO: move this to return_func_get_trending
static int
_luafunc_trending_result(lua_State *L)
{
	//shamuneko_state_t *st = lua_touserdata(L, lua_upvalueindex(1));
	struct _result_data *internal = lua_touserdata(L, 1);
	get_trending_callback_t callback = internal->callback;
	struct shamuneko_trending_result* results;

	lua_len(L, 2);
	int len = lua_tonumber(L, -1);
	lua_pop(L, 1);

	results = calloc(len, sizeof(struct shamuneko_trending_result));

	for (int i = 1; i <= len; ++i)
	{
		struct shamuneko_trending_result *result = results + (i-1);
		lua_geti(L, 2, i);

		SET_LFIELD(result, id);
		SET_LFIELD(result, name);
		SET_LFIELD(result, description);
		SET_LFIELD(result, cover);

		lua_pop(L, 1);
	}

	if (callback) callback(results, len, internal->data);

	return 0;
}


void
shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t data_len)
{
	struct _shamuneko_request *req = internal;
	int noop;
	lua_State *co = req->co;
	shamuneko_state_t *st = req->st;

	// TODO move below to ext func
	lua_pushthread(co);
	lua_gettable(co, LUA_REGISTRYINDEX);
	struct _result_data *result = lua_touserdata(co, -1);
	lua_pop(co, 2);

	if (!ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
		lua_pushthread(co);
	lua_pushlstring(co, data, data_len);
	//lua_pcall(_L, 1, LUA_MULTRET, 0);
	if (!ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
	{
		if (lua_resume(co, _L, 1, &noop) == LUA_OK)
			result->return_func(co, result);
		luaL_unref(co, LUA_REGISTRYINDEX, req->thread_ref);
	}
}

shamuneko_state_t*
shamuneko_new(shamuneko_flags_t flags, struct shamuneko_http_funcs funcs)
{
	shamuneko_state_t *st = calloc(1, sizeof(shamuneko_state_t));
	if (!st)
		return NULL;
	st->flags = flags;
	st->funcs = funcs;

	_L = luaL_newstate();

	create_httpsession_table(st);
	create_htmlparser_table(st);
	create_json_funcs(st);

	// TODO: we're not going to be exposing all of this
	luaL_openlibs(_L);

	return st;
}

void
shamuneko_destroy(shamuneko_state_t *st)
{
	lua_close(_L);
}
