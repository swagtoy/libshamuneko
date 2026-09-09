#include <stdlib.h>
#include "shamuneko.h"
#include "http.h"

#include "shamuneko_private.h"

shamuneko_state_t*
shamuneko_new(struct shamuneko_http_funcs funcs)
{
	shamuneko_state_t *st = calloc(1, sizeof(shamuneko_state_t));
	if (!st)
		return NULL;
	st->funcs = funcs;

	_L = luaL_newstate();

	create_httpsession_table(st);

	luaL_openlibs(_L);

	return st;
}

void
shamuneko_destroy(shamuneko_state_t *st)
{
	lua_close(_L);
}
