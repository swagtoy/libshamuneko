#ifndef _UTIL_H_
#define _UTIL_H_

#include "shamuneko_private.h"

// some helpful lua stack guarding
#if !defined(NDEBUG) && (defined(__GNUC__) || defined(__clang__))
	struct guard_lua_stack_info {
		lua_State* L;
		int top;
		int offset;
		char const *file;
		int line;
		char const *function;
	};

	void guard_lua_stack_assert(void *ptr);

	// for cases where arguments are pushed (and returned), we set OFFSET
#	define GUARD_LUA_STACK_INIT(L) \
		__attribute__((unused)) struct guard_lua_stack_info __GLSINFO = \
			{ L, lua_gettop(L), 0, __FILE__, __LINE__, __FUNCTION__ }
	// in some instances, the result could be different depending on
	// the control flow, so we allow checking manually too
	// Also: we restore the offset when done, to allow checking with
	// GUARD_LUA_STACK too.
#	define GUARD_LUA_STACK_CHECK(OFFSET) \
	if (1) { \
		int tmp = __GLSINFO.offset; \
		 __GLSINFO.offset = OFFSET; \
		guard_lua_stack_assert(&__GLSINFO); \
		__GLSINFO.offset = tmp; \
	} (void)0
#	define GUARD_LUA_STACK(L, OFFSET) \
		__attribute__((cleanup(guard_lua_stack_assert))) GUARD_LUA_STACK_INIT(L); \
		__GLSINFO.offset = OFFSET
#else
#	define GUARD_LUA_STACK(L, OFFSET)
#	define GUARD_LUA_STACK_BEGIN(L, OFFSET)
#	define GUARD_LUA_STACK_END(OFFSET)
#endif

struct _result_data* data_from_regidx(lua_State *L);

#endif // _UTIL_H_
