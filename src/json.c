#include <cjson/cJSON.h>
#include "json.h"

static void
_serialize_json_recurse(lua_State *L, cJSON *json)
{
	cJSON *curr;
	int idx;
	if (cJSON_IsBool(json))
		lua_pushboolean(L, cJSON_IsTrue(json));
	else if (cJSON_IsNumber(json))
		lua_pushnumber(L, json->valuedouble);
	else if (cJSON_IsString(json))
		lua_pushstring(L, json->valuestring);
	else if (cJSON_IsNull(json))
		lua_pushnil(L);
	else if (cJSON_IsArray(json))
	{
		lua_newtable(L);
		curr = json->child;
		idx = 0;
		for (; curr; curr = curr->next)
		{
			_serialize_json_recurse(L, curr);
			lua_rawseti(L, -2, ++idx);
		}
	}
	else if (cJSON_IsObject(json))
	{
		lua_newtable(L);
		curr = json->child;
		for (; curr; curr = curr->next)
		{
			lua_pushstring(L, curr->string);
			_serialize_json_recurse(L, curr);
			lua_settable(L, -3);
		}
	}
}

static int
_luafunc_json_parse(lua_State *L)
{
	size_t len;
	char const *data = luaL_checklstring(L, 1, &len);
	cJSON *json = cJSON_ParseWithLength(data, len);
	if (!json)
	{
		DEBUGF("JSON parsing failed here -> %s", cJSON_GetErrorPtr());
		return 0;
	}

	_serialize_json_recurse(L, json);

	cJSON_Delete(json);
	return 1;
}

static int
_luafunc_json_dump(lua_State *L)
{
	size_t len;
	char const *data = luaL_checklstring(L, 1, &len);
	cJSON *json = cJSON_ParseWithLength(data, len);
	if (!json)
	{
		DEBUGF("JSON parsing failed here -> %s", cJSON_GetErrorPtr());
		return 0;
	}
	int format = lua_isboolean(L, 2) ? lua_toboolean(L, 2) : 0;

	lua_pushstring(L, format ? cJSON_Print(json) : cJSON_PrintUnformatted(json));

	cJSON_Delete(json);
	return 1;
}


static struct luaL_Reg _json_funcs[] = {
	{ "parse", _luafunc_json_parse },
	// honestly just wanted an extra function to justify the table existing :P
	{ "dump", _luafunc_json_dump },
	{ 0, 0 }
};

void
create_json_funcs(shamuneko_state_t *st)
{
	lua_newtable(_L);
	luaL_setfuncs(_L, _json_funcs, 0);
	lua_setglobal(_L, "json");
}
