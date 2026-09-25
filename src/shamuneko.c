#include <stdlib.h>
#include "shamuneko.h"
#include "http.h"
#include "html.h"
#include "json.h"
#include "returnfuncs.h"
#include "util.h"

#include "shamuneko_private.h"

void
shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t data_len)
{
	struct _shamuneko_request *req = internal;
	int noop;
	lua_State *co = req->co;
	GUARD_LUA_STACK_INIT(co);
	shamuneko_state_t *st = req->st;

	struct _result_data *result = data_from_regidx(co);

	if (!ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
		lua_pushthread(co);
	lua_pushlstring(co, data, data_len);

	if (!ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
	{
		if (lua_resume(co, _L, 1, &noop) == LUA_OK)
			result->return_func(co, result);
		luaL_unref(co, LUA_REGISTRYINDEX, req->thread_ref);
	}
	else
		GUARD_LUA_STACK_CHECK(1);
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
