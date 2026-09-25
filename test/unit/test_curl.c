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

#ifndef PROJ_DIR
#	define PROJ_DIR
#	error No PROJ_DIR defined!
#endif

struct memory {
	char *data;
	size_t size;
};

static size_t
_write(char *data, size_t size, size_t nmemb, void *mydata)
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
_handle_req_func(void *odata, char const *url, shamuneko_request_t *internal)
{
	CURL *curl = odata;
	struct memory chunk = { 0 };

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_PRIVATE, internal);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

	curl_easy_perform(curl);
	internal = NULL; // for theory

	// Now... let's pretend that we don't have shamuneko_internal at this point
	curl_easy_getinfo(curl, CURLINFO_PRIVATE, &internal);

	shamuneko_process_request(internal, chunk.data, chunk.size);

	free(chunk.data);

	return 0;
}

static void
_destroy_session_func(void *odata)
{
	printf("DESTROYING SESSION %p\n", odata);
	curl_easy_cleanup(odata);
}

////////////

static void
_get_trending_cb(struct shamuneko_trending_result *result, size_t len, void *data)
{
	puts("Trending callback received...");
}

static void
_get_pages_cb(struct shamuneko_pages_result *result, size_t len, void *data)
{
	puts("Pages callback received...");

	for (int i = 0; i < len; ++i)
	{
		printf("\tPage %d: %s\n", i+1, result[i].img_url);
	}
}


int
main()
{
	curl_global_init(CURL_GLOBAL_ALL);

	/* NOTE: Since we use curl easy handles in our request functions,
	 *  we MUST set SHAMUNEKO_FLAG_SYNCHRONOUS.
	 *
	 * Why? Well if not set, internally, the Lua coroutines created
	 *  for this function would typically 'yield', and we'd call
	 *  shamuneko_process_request later in, say, the application event
	 *  loop. Since we are performing the requests synchronously in
	 *  the same request callback function with a single curl easy
	 *  socket, we instruct libshamuneko to not yield, and instead
	 *  just block.
	 */
	shamuneko_state_t *state = shamuneko_new(SHAMUNEKO_FLAG_SYNCHRONOUS,
		(struct shamuneko_http_funcs){
			.create_session = _create_session_func,
			.request = _handle_req_func,
			.destroy_session = _destroy_session_func,
		});
	assert(state != NULL);

#if 0
	shamuneko_module_t *module =
		shamuneko_module_load(state, TESTS_DIR "modules/example1.lua");

	shamuneko_module_search(module, "red cat ramen");

	shamuneko_module_destroy(module);
#endif

#if 1
	shamuneko_module_t *pepperandcarrot =
		shamuneko_module_load(state, PROJ_DIR "modules/pepper&carrot/main.lua");

	//shamuneko_module_search(pepperandcarrot, "help");
	shamuneko_module_get_trending(pepperandcarrot, _get_trending_cb, NULL);
	shamuneko_module_get_pages(pepperandcarrot, "peppercarrot", 0, _get_pages_cb, NULL);

	shamuneko_module_destroy(pepperandcarrot);
#endif

	shamuneko_destroy(state);
	return 0;
}
