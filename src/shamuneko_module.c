#include <stdlib.h>
#include "shamuneko_module.h"

#include "shamuneko_private.h"

#define _MODULE_ST shamuneko_state_t *st = module->st

static shamuneko_module_t*
_create_module_storage(shamuneko_state_t *st)
{
	shamuneko_module_t *storage = calloc(1, sizeof(shamuneko_module_t));

	lua_getfield(_L, -1, "name");
	storage->name = lua_tostring(_L, -1);
	lua_pop(_L, 1);
	lua_getfield(_L, -1, "version");
	storage->version = lua_tonumber(_L, -1);
	lua_pop(_L, 1);

	DEBUGF("name: %s", storage->name);
	DEBUGF("version: %d", storage->version);

	storage->tref = luaL_ref(_L, LUA_REGISTRYINDEX);
	storage->st = st;

	assert(lua_gettop(_L) == 0);
	return storage;
}

static void
_push_mod_table(lua_State *L, shamuneko_module_t *module)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, module->tref);

	assert(lua_istable(L, -1));
}

void
shamuneko_module_search(shamuneko_module_t *module,
                        char *query)
{
	_MODULE_ST;

	int noop;
	lua_State *co = lua_newthread(_L);
	_push_mod_table(co, module);
	lua_getfield(co, -1, "search");
	// TODO: push search callback
	lua_pushstring(co, query);
	lua_resume(co, _L, 1, &noop);
	lua_pop(_L, 1);

	_ASSERT_TOP;
}

void
shamuneko_module_get_trending(shamuneko_module_t *module,
                              get_trending_callback_t cb,
							  void *data)
{
	_MODULE_ST;

	int noop = 0;
	lua_State *co = lua_newthread(_L);
	_push_mod_table(co, module);
	lua_getfield(co, -1, "get_trending");
	struct _result_data *result = lua_newuserdata(co, sizeof(struct _result_data));
	result->callback = cb;
	result->data = data;
	result->called = 0;
	// TODO: check if this TODO is still valid with coroutines
	// TODO: A metatable with a __gc hook (i suppose) around the lua
	// userdata here would be needed. if result->called == 0, but we
	// GC, we could call the callback with NULL so the user could
	// cleanup any possible void* data.
	lua_resume(co, _L, 1, &noop);
	lua_pop(_L, 1);
	_ASSERT_TOP;
}

shamuneko_module_t*
shamuneko_module_load(shamuneko_state_t *st,
                      char const        *filename)
{
	int ret = luaL_dofile(_L, filename);
	DEBUG_LUA_IF_ERROR(ret, _L, return NULL);

	return _create_module_storage(st);
}

void
shamuneko_module_destroy(shamuneko_module_t *module)
{
	luaL_unref(module->st->L, LUA_REGISTRYINDEX, module->tref);
	free(module);
}
