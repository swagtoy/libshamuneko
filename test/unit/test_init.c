#include <stdio.h>
#include <shamuneko.h>
#include <assert.h>

#ifndef TESTS_DIR
#	define TESTS_DIR
#	error No TESTS_DIR defined!
#endif

int
main()
{
	shamuneko_state_t* state = shamuneko_new();
	assert(state != NULL);

	assert(shamuneko_load_module(state, TESTS_DIR "modules/example1.lua") != NULL);

	shamuneko_destroy(state);
	return 0;
}
