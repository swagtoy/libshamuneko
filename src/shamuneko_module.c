#include <assert.h>
#include <stdlib.h>
#include "shamuneko_module.h"

#include "shamuneko_private.h"

#define _ASSERT_TOP do { assert(lua_gettop(_L) == 0); } while(0)
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
_push_mod_table(shamuneko_module_t *module)
{
	_MODULE_ST;
	lua_rawgeti(_L, LUA_REGISTRYINDEX, module->tref);

	assert(lua_istable(_L, -1));
}

void
shamuneko_module_search(shamuneko_module_t *module,
                        char *query)
{
	_MODULE_ST;
	_push_mod_table(module);
	lua_getfield(_L, -1, "search");
	lua_pushstring(_L, query);
	lua_pcall(_L, 1, LUA_MULTRET, 0);
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
