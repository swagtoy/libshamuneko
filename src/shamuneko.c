#include <math.h>
#include <stdlib.h>
#include "shamuneko.h"
#include "http.h"
#include "html.h"
#include "json.h"
#include "util.h"
#include "miniz.h"

#include "shamuneko_private.h"

static void
_process_request_page(shamuneko_request_t *internal, char *data, size_t data_len)
{
	struct _shamuneko_request_page *req = (struct _shamuneko_request_page*)internal;
	assert (req->base.type == SHAMUNEKO_REQUEST_TYPE_PAGE);
	int noop;
	lua_State *co = req->co;
	GUARD_LUA_STACK_INIT(co);
	shamuneko_state_t *st = req->st;

	struct _result_data *result = data_from_regidx(co);

	if (!ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
		lua_pushthread(co);
	lua_pushlstring(co, data, data_len);
	// TODO: custom free func
	free(data);

	if (!ST_HAS_FLAG(SHAMUNEKO_FLAG_SYNCHRONOUS))
	{
		if (lua_resume(co, _L, 1, &noop) == LUA_OK)
			result->return_func(co, result);
		luaL_unref(co, LUA_REGISTRYINDEX, req->thread_ref);
	}
	else
		GUARD_LUA_STACK_CHECK(1);
}

static void
_process_request_data(shamuneko_request_t *internal, char *data, size_t data_len)
{
	struct _shamuneko_request_data *req = (struct _shamuneko_request_data*)internal;
	assert(req->base.type == SHAMUNEKO_REQUEST_TYPE_DATA);
	struct _shamuneko_request_data_shared *shared = req->shared;

	shared->pages[req->page_idx].img = data;
	shared->pages[req->page_idx].img_size = data_len;

	if (--shared->pages_left == 0)
	{
		shared->callback(shared->pages, shared->pages_len, shared->callback_data);

		// we are done!
		for (int i = 0; i < shared->pages_len; ++i)
		{
			free((void*)req->shared->pages[i].img_url);
			// TODO: custom free func
			free(req->shared->pages[i].img);
		}
		free(req->shared->pages);
		free(req->shared);
	}

	// this request data isn't needed anymore.
	free(req);
}

void
shamuneko_process_request(shamuneko_request_t *internal, char *data, size_t data_len)
{
	switch (internal->type)
	{
	case SHAMUNEKO_REQUEST_TYPE_PAGE: _process_request_page(internal, data, data_len); break;
	case SHAMUNEKO_REQUEST_TYPE_DATA: _process_request_data(internal, data, data_len); break;
	default:
		assert(!"shouldn't end up here :(");
	}
}

shamuneko_state_t*
shamuneko_new(shamuneko_flags_t flags, struct shamuneko_http_funcs funcs)
{
	shamuneko_state_t *st = calloc(1, sizeof(shamuneko_state_t));
	if (!st)
		return NULL;
	st->flags = flags;
	st->funcs = funcs;

	_L = luaL_newstate();

	create_httpsession_table(st);
	create_htmlparser_table(st);
	create_json_funcs(st);

	// TODO: we're not going to be exposing all of this
	luaL_openlibs(_L);

	return st;
}

void
shamuneko_download_pages(shamuneko_state_t *st,
                         struct shamuneko_pages_result *pages,
                         size_t len,
                         download_pages_callback_t cb,
                         void *data)
{
	if (!st->funcs.request)
		return;
	// we'll share this around until we run out of pages
	struct _shamuneko_request_data_shared *shared = calloc(1, sizeof(struct _shamuneko_request_data_shared));
	shared->st = st;
	shared->pages = calloc(len, sizeof(struct shamuneko_pages_result));
	shared->pages_len = len;
	shared->pages_left = len;
	shared->callback = cb;
	shared->callback_data = data;

	for (size_t i = 0; i < len; ++i)
	{
		struct _shamuneko_request_data *req = calloc(1, sizeof(struct _shamuneko_request_data));
		req->base.type = SHAMUNEKO_REQUEST_TYPE_DATA;
		req->shared = shared;
		req->page_idx = i;
		shared->pages[i].img_url = strdup(pages[i].img_url);

		st->funcs.request(NULL /* NG :( */, pages[i].img_url, (shamuneko_request_t*)req);
	}
}

void
shamuneko_pages_to_cbz_file(char const *output_filename, struct shamuneko_pages_result *pages, size_t len)
{
	// TODO: lotta error checking
	char filename[16];
	mz_zip_archive zip = { 0 };
	mz_zip_writer_init_file(&zip, output_filename, 0);

	for (int i = 0; i < len; ++i)
	{
		// zero pad with one more than the number of pages
		int padding = floorf(log10(len))+2;
		snprintf(filename, sizeof(filename)-1, "%0*d.jpg", padding, i);
		mz_zip_writer_add_mem(&zip, filename, pages[i].img, pages[i].img_size, MZ_DEFAULT_COMPRESSION);
	}
	mz_zip_writer_finalize_archive(&zip);
	mz_zip_writer_end(&zip);

}

void
shamuneko_destroy(shamuneko_state_t *st)
{
	lua_close(_L);
}
