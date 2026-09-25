#include <stdlib.h>
#include "shamuneko_module.h"
#include "returnfuncs.h"
#include "util.h"

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
#if 0
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
#endif
}

void
shamuneko_module_get_trending(shamuneko_module_t *module,
                              get_trending_callback_t cb,
                              void *data)
{
	_MODULE_ST;
	GUARD_LUA_STACK(_L, 0);

	int noop = 0;
	lua_State *co = lua_newthread(_L);
	_push_mod_table(co, module);
	lua_getfield(co, -1, "get_trending");
	lua_remove(co, -2); // get our table off the table
	struct _result_data *result = lua_newuserdata(co, sizeof(struct _result_data));
	result->callback = cb;
	result->data = data;
	result->return_func = return_func_get_trending;
	// we keep track of how we must return by attaching data to this
	// thread, with the thread as the key ;]
	lua_pushthread(co);
	lua_pushvalue(co, -2); // our new userdata
	lua_settable(co, LUA_REGISTRYINDEX);
	lua_pop(co, 1);

	// TODO: check if this TODO is still valid with coroutines
	// TODO: A metatable with a __gc hook (i suppose) around the lua
	// userdata here would be needed. if result->called == 0, but we
	// GC, we could call the callback with NULL so the user could
	// cleanup any possible void* data.
	if (lua_resume(co, _L, 0, &noop) == LUA_OK)
	{
		result->return_func(co, result);
	}
	lua_pop(_L, 1);
}

void
shamuneko_module_get_pages(shamuneko_module_t *module,
                           char const *id,
                           unsigned chapter,
                           get_pages_callback_t cb,
                           void *data)
{
	_MODULE_ST;
	GUARD_LUA_STACK(_L, 0);

	int noop = 0;
	lua_State *co = lua_newthread(_L);
	_push_mod_table(co, module);
	lua_getfield(co, -1, "get_pages");
	lua_remove(co, -2); // get our table off the table
	struct _result_data *result = lua_newuserdata(co, sizeof(struct _result_data));
	result->callback = cb;
	result->data = data;
	result->return_func = return_func_get_pages;
	// we keep track of how we must return by attaching data to this
	// thread, with the thread as the key ;]
	lua_pushthread(co);
	lua_pushvalue(co, -2); // our new userdata
	lua_settable(co, LUA_REGISTRYINDEX);
	lua_pop(co, 1);
	// args
	lua_pushstring(co, id);
	lua_pushinteger(co, chapter);

	// see last comment
	if (lua_resume(co, _L, 2, &noop) == LUA_OK)
	{
		result->return_func(co, result);
	}
	lua_pop(_L, 1);
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
