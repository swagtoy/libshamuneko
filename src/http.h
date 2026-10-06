#ifndef _HTTP_H_
#define _HTTP_H_

#ifndef SHAMUNEKO_AMALGAMATION_HEADER
#	include <lua.h>
#endif
#include "shamuneko_private.h"

SHAMUNEKO_PRIVATE int  luafunc_create_httpsession(lua_State *L);
SHAMUNEKO_PRIVATE void create_httpsession_table(shamuneko_state_t *st);

#endif // _HTTP_H_
