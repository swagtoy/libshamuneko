#include <stdlib.h>
#include "util.h"

#if !defined(NDEBUG) && (defined(__GNUC__) || defined(__clang__))

void guard_lua_stack_assert(void *ptr)
{
	struct guard_lua_stack_info *info = ptr;
	int currtop = lua_gettop(info->L);
	if (currtop != (info->top + info->offset))
	{
		fprintf(stderr, "%s:%d: FAIL! Lua stack mismatch in %s.\n"
		                "  Expected %d, but curent stack is %d + %d offset\n",
						info->file, info->line, info->function, currtop, info->top, info->offset);
		abort();
	}
}

#endif

struct _result_data*
data_from_regidx(lua_State *L)
{
	lua_pushthread(L);
	lua_gettable(L, LUA_REGISTRYINDEX);
	struct _result_data *result = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return result;
}
