#include <stdlib.h>
#include <stdio.h>
#include <shamuneko.h>
#include <assert.h>

#ifndef TESTS_DIR
#	define TESTS_DIR
#	error No TESTS_DIR defined!
#endif

void*
_create_session_func()
{
	int *data = malloc(sizeof(int));
	printf("CREATING SESSION %p\n", data);
	*data = rand();

	return data;
}

int
_handle_req_func(void *odata, char const *url, shamuneko_request_t *shamuneko_internal)
{
	int *data = odata;
	printf("The user requested[%d]: %s\n", *data, url);

	return 0;
}

void
_destroy_session_func(void *odata)
{
	printf("DESTROYING SESSION %p\n", odata);
	free(odata);
}

int
main()
{
	shamuneko_state_t *state = shamuneko_new((struct shamuneko_http_funcs){
		.create_session = _create_session_func,
		.request = _handle_req_func,
		.destroy_session = _destroy_session_func,
	});
	assert(state != NULL);

	shamuneko_module_t *module =
		shamuneko_module_load(state, TESTS_DIR "modules/example1.lua");

	shamuneko_module_search(module, "red cat ramen");

	shamuneko_module_destroy(module);

	shamuneko_destroy(state);
	return 0;
}
