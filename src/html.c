#include <string.h>
#include <stdlib.h>
#include <lua5.4/lauxlib.h>
#include "html.h"
#include <libxml/HTMLparser.h>

#define _METATABLE_NAME "HTMLparser"

struct htmlparser_data
{
	xmlDoc *doc;
	xmlNode *node;
	// refcount for doc
	int *ref;
};

static void
_cleanup_htmlparser_data(struct htmlparser_data *data)
{
	xmlFreeDoc(data->doc);
	// node shouldn't need to be free'd, since it's part of the doc...
	free(data->ref);
}

static int
_push_node(lua_State *L, struct htmlparser_data *d, xmlNode *node)
{
	// i.e. node->next would be nil
	if (!node)
		return 0;

	struct htmlparser_data *tbl = lua_newuserdata(L, sizeof(struct htmlparser_data));
	tbl->doc = d->doc;
	tbl->node = node;
	++*(tbl->ref = d->ref);

	luaL_getmetatable(L, _METATABLE_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

/* -- the two methods below are for such
 * for elem in node:iter() do
 *     print(elem:name())
 * end
 */
static int
_luafunc_htmlparser_iter_step(lua_State *L)
{
	struct htmlparser_data *d = luaL_checkudata(L, (lua_isnil(L, 2) ? 1 : 2), _METATABLE_NAME);
	return _push_node(L, d, (lua_isnil(L, 2) ? d->node : d->node->next));
}

static int
_luafunc_htmlparser_iter(lua_State *L)
{
	lua_pushcclosure(L, _luafunc_htmlparser_iter_step, 0);
	lua_pushvalue(L, 1); // idx:1 == our metatable
	lua_pushnil(L);

	return 3;
}

#define _LUAFUNC_GEN_PUSH_NODE_FUNC(OP, IS_RECURSIVE) \
	static int \
	_luafunc_htmlparser_ ## OP (lua_State *L) \
	{ \
		struct htmlparser_data *d = luaL_checkudata(L, 1, _METATABLE_NAME); \
		xmlNode *node = d->node->OP; \
		if (IS_RECURSIVE && lua_isinteger(L, -1)) { \
			lua_Integer amt = lua_tointeger(L, -1); \
			lua_pop(L, 1); \
			/* wouldn't make sense */ \
			if (amt <= 0) \
				return 0; \
			while (--amt) { \
				if (node->OP == NULL) \
					return 0; \
				node = node->OP; \
			} \
		} \
		return _push_node(L, d, node); \
	}

_LUAFUNC_GEN_PUSH_NODE_FUNC(next, 1)
_LUAFUNC_GEN_PUSH_NODE_FUNC(children, 1)
_LUAFUNC_GEN_PUSH_NODE_FUNC(parent, 1)
_LUAFUNC_GEN_PUSH_NODE_FUNC(prev, 1)
_LUAFUNC_GEN_PUSH_NODE_FUNC(last, 0)

#define _LUAFUNC_GEN_PUSH_STRING_FUNC(OP) \
	static int \
	_luafunc_htmlparser_ ## OP (lua_State *L) \
	{ \
		struct htmlparser_data *d = luaL_checkudata(L, 1, _METATABLE_NAME); \
		if (!d->node->OP) \
			return 0; \
		lua_pushstring(L, (char*)d->node->OP); \
		return 1; \
	}

_LUAFUNC_GEN_PUSH_STRING_FUNC(content)
_LUAFUNC_GEN_PUSH_STRING_FUNC(name)

static int
_luafunc_htmlparser_type(lua_State *L)
{
	struct htmlparser_data *d = luaL_checkudata(L, 1, _METATABLE_NAME);
	lua_pushinteger(L, d->node->type);
	return 1;
}

static int
_luafunc_htmlparser_attr(lua_State *L)
{
	struct htmlparser_data *d = luaL_checkudata(L, 1, _METATABLE_NAME);
	xmlAttr *props = d->node->properties;
	if (!props)
		return 0;

	lua_newtable(L);
	do
	{
		lua_pushstring(L, (char*)props->name);
		lua_pushstring(L, (char*)props->children->content);
		lua_rawset(L, -3);
	}
	while ((props = props->next));
	//d->node->properties
	//lua_pushstring(L, )
	return 1;
}

static int
_luafunc_htmlparser_create(lua_State *L)
{
	struct htmlparser_data *d = lua_newuserdata(L, sizeof(struct htmlparser_data));
	d->ref = malloc(sizeof(int));
	*d->ref = 1;

	size_t len;
	char const *str = lua_tolstring(L, -2, &len);

	d->doc = htmlReadMemory(str, len, "shamuneko.html", NULL, 0);
	if (!d->doc)
	{
		DEBUGF("Failed to parse HTML document \"%.*s%s\"",
			(len > 70 ? 70 : (int)len), str, (len > 70 ? "..." : ""));
		// TODO: cleanup
		_cleanup_htmlparser_data(d);
		return 0;
	}

	d->node = xmlDocGetRootElement(d->doc);
	// TODO: null?

	luaL_getmetatable(L, _METATABLE_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static int
_luafunc_htmlparser_destroy(lua_State *L)
{
	struct htmlparser_data *d = luaL_checkudata(L, 1, _METATABLE_NAME);
	if (--(*d->ref) == 0)
	{
		_cleanup_htmlparser_data(d);
		DEBUG("Freeing xml document");
	}

	return 0;
}

static struct luaL_Reg _htmlparser_meths[] = {
	{ "type", _luafunc_htmlparser_type },

	{ "next", _luafunc_htmlparser_next },
	{ "children", _luafunc_htmlparser_children },
	{ "parent", _luafunc_htmlparser_parent },
	{ "prev", _luafunc_htmlparser_prev },

	{ "last", _luafunc_htmlparser_last },

	{ "name", _luafunc_htmlparser_name },
	{ "content", _luafunc_htmlparser_content },
	{ "attr", _luafunc_htmlparser_attr },

	{ "iter", _luafunc_htmlparser_iter },

	{ "__gc", _luafunc_htmlparser_destroy },
	{ 0, 0 }
};

static struct luaL_Reg _htmlparser_funcs[] = {
	{ "new", _luafunc_htmlparser_create },
	{ 0, 0 }
};

void
create_htmlparser_table(shamuneko_state_t *st)
{
	luaL_newmetatable(_L, _METATABLE_NAME);

	lua_pushliteral(_L, "__index");
	lua_pushvalue(_L, -2);
	lua_rawset(_L, -3);

	// setup methods and funcs
	luaL_setfuncs(_L, _htmlparser_meths, 0);
	lua_pop(_L, 1);

	luaL_newlib(_L, _htmlparser_funcs);

#define LUA_XML_ICONST(name) lua_pushstring(_L, #name); \
		lua_pushinteger(_L, XML_ ## name); \
		lua_rawset(_L, -3);

	// these do NOT need to all be defined, but we'll do it anyway; its a bit harmless
	LUA_XML_ICONST(ELEMENT_NODE);
	LUA_XML_ICONST(ATTRIBUTE_NODE);
	LUA_XML_ICONST(TEXT_NODE);
	LUA_XML_ICONST(CDATA_SECTION_NODE);
	LUA_XML_ICONST(ENTITY_REF_NODE);
	LUA_XML_ICONST(PI_NODE);
	LUA_XML_ICONST(COMMENT_NODE);
	LUA_XML_ICONST(DOCUMENT_NODE);
	LUA_XML_ICONST(DOCUMENT_TYPE_NODE);
	LUA_XML_ICONST(DOCUMENT_FRAG_NODE);
	LUA_XML_ICONST(NOTATION_NODE);
	LUA_XML_ICONST(HTML_DOCUMENT_NODE);
	LUA_XML_ICONST(DTD_NODE);
	LUA_XML_ICONST(ELEMENT_DECL);
	LUA_XML_ICONST(ATTRIBUTE_DECL);
	LUA_XML_ICONST(ENTITY_DECL);
	LUA_XML_ICONST(NAMESPACE_DECL);
	LUA_XML_ICONST(XINCLUDE_START);
	LUA_XML_ICONST(XINCLUDE_END);
#undef LUA_CONST

	lua_setglobal(_L, "htmlparser");

	_ASSERT_TOP;
}
