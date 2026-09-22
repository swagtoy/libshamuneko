#include "util.h"

struct _result_data*
data_from_regidx(lua_State *L)
{
	lua_pushthread(L);
	lua_gettable(L, LUA_REGISTRYINDEX);
	struct _result_data *result = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return result;
}
