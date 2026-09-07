#ifndef _SHAMUNEKO_H_
#define _SHAMUNEKO_H_

#include "shamuneko_module.h"

typedef struct _shamuneko_state shamuneko_state_t;
struct _shamuneko_state;

shamuneko_state_t*  shamuneko_new(void);
void                shamuneko_destroy(shamuneko_state_t *state);

#endif // _SHAMUNEKO_H_
