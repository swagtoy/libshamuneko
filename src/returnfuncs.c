#include "returnfuncs.h"

int
return_func_get_trending(lua_State *L, struct _result_data *result)
{
	puts("Return from sender!\n");
	return 0;
}
