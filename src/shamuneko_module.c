#include <stdlib.h>
#include "shamuneko_module.h"

#include "shamuneko_private.h"

static shamuneko_module_t*
_create_module_storage(shamuneko_state_t *st)
{
	shamuneko_module_t *storage = calloc(1, sizeof(shamuneko_module_t));

	int top = lua_gettop(_L);
	lua_getfield(_L, top, "name");
	storage->name = lua_tostring(_L, -1);
	lua_getfield(_L, top, "version");
	storage->version = lua_tonumber(_L, -1);

	DEBUGF("name: %s", storage->name);
	DEBUGF("version: %d", storage->version);
	return storage;
}

shamuneko_module_t*
shamuneko_load_module(shamuneko_state_t *st,
                      char const        *filename)
{
	int ret = luaL_dofile(_L, filename);
	DEBUG_LUA_IF_ERROR(ret, _L, return NULL);

	return _create_module_storage(st);
}

void
shamuneko_destroy_module(shamuneko_module_t *module)
{
	free(module);
}
