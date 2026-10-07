#ifndef _SHAMUNEKO_PRIVATE_H_
#define _SHAMUNEKO_PRIVATE_H_

#include <stdint.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "shamuneko_types.h"

#if defined(__GNUC__) && !defined(SHAMUNEKO_AMALGAMATION_HEADER)
#	define SHAMUNEKO_PRIVATE __attribute__((visibility("hidden")))
#else
#	define SHAMUNEKO_PRIVATE
#endif

#define _L (st->L)

struct _shamuneko_state
{
	lua_State *L;
	shamuneko_flags_t flags;
	struct shamuneko_http_funcs funcs;
};

struct _shamuneko_module
{
	struct _shamuneko_state *st;
	int tref;
	char const *name;
	int version;
};

enum _shamuneko_request_type
{
	SHAMUNEKO_REQUEST_TYPE_PAGE,
	SHAMUNEKO_REQUEST_TYPE_DATA,
};

struct _shamuneko_request
{
	enum _shamuneko_request_type type;
};

struct _shamuneko_request_page  // type for fetching pages of urls
{
	struct _shamuneko_request base;
	lua_State *co;
	shamuneko_state_t *st;
	int thread_ref;
};

struct _shamuneko_request_data_shared
{
	shamuneko_state_t *st;
	struct shamuneko_pages_result *pages;
	size_t pages_len;
	size_t pages_left;

	download_pages_callback_t callback;
	void *callback_data;
};

struct _shamuneko_request_data  // type for fetching data like png's
{
	struct _shamuneko_request base;
	struct _shamuneko_request_data_shared *shared;
	size_t page_idx;
};

struct _result_data
{
	void *callback;
	void *data;
	int called;
	int (*return_func)(lua_State *L, struct _result_data *data);
};

#define ST_HAS_FLAG(FLAG) ((st->flags & FLAG) == FLAG)
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
