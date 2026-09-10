#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <shamuneko.h>
#include <assert.h>
#include <curl/curl.h>

#ifndef TESTS_DIR
#	define TESTS_DIR
#	error No TESTS_DIR defined!
#endif

struct memory {
	char *data;
	size_t size;
};

static size_t _write(char *data, size_t size, size_t nmemb, void *mydata)
{
	struct memory *mem = mydata;

	char *ptr = realloc(mem->data, mem->size + nmemb + 1);
	if (!ptr) return 0;

	mem->data = ptr;
	memcpy(mem->data + mem->size, data, nmemb);
	mem->size += nmemb;
	mem->data[mem->size] = '\0';
	return nmemb;
}

static void*
_create_session_func()
{
	// could be done in _handle_req_func, but we're only proving a point
	return curl_easy_init();
}

static int
_handle_req_func(void *odata, char const *url, shamuneko_request_t *shamuneko_internal)
{
	CURL *curl = odata;
	struct memory chunk = { 0 };

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_PRIVATE, shamuneko_internal);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

	curl_easy_perform(curl);
	shamuneko_internal = NULL; // for theory

	// Now... let's pretend that we don't have shamuneko_internal at this point
	curl_easy_getinfo(curl, CURLINFO_PRIVATE, &shamuneko_internal);
	printf("Got data: %s\n", chunk.data);
	shamuneko_process_request(shamuneko_internal, chunk.data, chunk.size);

	free(chunk.data);

	return 0;
}

static void
_destroy_session_func(void *odata)
{
	printf("DESTROYING SESSION %p\n", odata);
	curl_easy_cleanup(odata);
}

int
main()
{
	curl_global_init(CURL_GLOBAL_ALL);

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
