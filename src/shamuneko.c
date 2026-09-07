#include <stdlib.h>
#include "shamuneko.h"

#include "shamuneko_private.h"

shamuneko_state_t*
shamuneko_new(void)
{
	shamuneko_state_t *st = calloc(1, sizeof(shamuneko_state_t));
	if (!st)
		return NULL;

	_L = luaL_newstate();
	// TODO: this won't be the case
	luaL_openlibs(_L);

	return st;
}


void
shamuneko_destroy(shamuneko_state_t *st)
{
	lua_close(_L);
}
