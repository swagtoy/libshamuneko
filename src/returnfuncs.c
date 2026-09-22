#include <stdlib.h>
#include "returnfuncs.h"

#define SET_LFIELD(result, field) \
	lua_getfield(L, -1, #field); \
	if (lua_isstring(L, -1)) result->field = lua_tostring(L, -1); \
	printf(#field ": %s\n", result->field); \
	lua_pop(L, 1)

int
return_func_get_trending(lua_State *L, struct _result_data *result)
{
	get_trending_callback_t callback = result->callback;
	struct shamuneko_trending_result* results;

	// XXX: something still on the stack here?
	int const res_tbl_idx = 2;
	lua_len(L, res_tbl_idx);
	int len = lua_tonumber(L, -1);
	lua_pop(L, 1);
	printf("Get trending: %d!\n", len);


	results = calloc(len, sizeof(struct shamuneko_trending_result));

	for (int i = 1; i <= len; ++i)
	{
		struct shamuneko_trending_result *result = results + (i-1);
		lua_geti(L, res_tbl_idx, i);

		SET_LFIELD(result, id);
		SET_LFIELD(result, name);
		SET_LFIELD(result, description);
		SET_LFIELD(result, cover);

		lua_pop(L, 1);
	}

	if (callback) callback(results, len, result->data);

	return 0;
}
