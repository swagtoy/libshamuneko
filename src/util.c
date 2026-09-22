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

static char const *
_luatype_to_string(int type)
{
	++type; // to account for LUA_TNONE
	char const * types[LUA_NUMTYPES+1] = {
		"none",
		"nil",
		"boolean",
		"lightuserdata",
		"number",
		"string",
		"table",
		"function",
		"userdata",
		"thread",
	};
	return types[type];
}

void
dump_stack(lua_State *L, int from, int to)
{
	if (to == -1)
		to = lua_gettop(L);
	assert(to > from);
	printf("LUA STACK DUMP -- from %d to %d\n", from, to);
	for (int i = from; i <= to; i++)
	{
		int type = lua_type(L, i);
		printf("  stack pos %i [%s]\n", i, _luatype_to_string(type));
		switch (type)
		{
		case LUA_TSTRING:
			printf("\tvalue: %s\n", lua_tostring(L, i));
			break;
		case LUA_TNUMBER:
			printf("\tvalue: %.02f\n", lua_tonumber(L, i));
			break;
		case LUA_TTABLE:
			printf("\tlua_rawlen: %lld\n", lua_rawlen(L, i));
			break;
		}
	}
	puts("END LUA STACK DUMP");
}

struct _result_data*
data_from_regidx(lua_State *L)
{
	GUARD_LUA_STACK(L, 0);
	lua_pushthread(L);
	lua_gettable(L, LUA_REGISTRYINDEX);
	struct _result_data *result = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return result;
}
