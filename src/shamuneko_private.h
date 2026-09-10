#ifndef _SHAMUNEKO_PRIVATE_H_
#define _SHAMUNEKO_PRIVATE_H_

#include <assert.h>
#include "shamuneko.h"
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#ifdef __GNUC__
#	define SHAMUNEKO_PRIVATE __attribute__((visibility("hidden")))
#else
#	define SHAMUNEKO_PRIVATE
#endif

#define _L (st->L)

struct _shamuneko_state
{
	lua_State *L;
	struct shamuneko_http_funcs funcs;
};

struct _shamuneko_module
{
	struct _shamuneko_state *st;
	int tref;
	char const *name;
	int version;
};

struct _shamuneko_request
{
	shamuneko_state_t *st;
	int callback_ref;
};

#define _ASSERT_TOP do { assert(lua_gettop(_L) == 0); } while(0)

// TODO: move this
#ifndef NDEBUG
#	define TOP DEBUGF("TOP: %d", (int)lua_gettop(_L))
#	define DEBUGF(msg, ...) printf("[DBG:" __FILE__ ":%d] " msg "\n", __LINE__, __VA_ARGS__)
#	define DEBUG(msg) puts(msg)
#	define DEBUG_LUA_IF_ERROR(status, luastate, expr) \
	if (status != LUA_OK) { \
		fprintf(stderr, "[DEBUG ERROR] %s\n", lua_tostring(luastate, -1)); \
		lua_pop(luastate, 1); expr; }
#else
#	define TOP
#	define DEBUGF(...)
#	define DEBUG(msg)
#	define DEBUG_LUA_IF_ERROR(...)
#endif

#endif
