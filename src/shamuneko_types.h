#ifndef _SHAMUNEKO_TYPES_H_
#define _SHAMUNEKO_TYPES_H_

#ifndef SHAMUNEKO_AMALGAMATION_HEADER
#	include <stdint.h>
#	include <stddef.h>
#endif

typedef uint8_t shamuneko_flags_t;
#define SHAMUNEKO_FLAG_SYNCHRONOUS (1<<2)

typedef struct _shamuneko_state shamuneko_state_t;
struct _shamuneko_state;

typedef struct _shamuneko_module shamuneko_module_t;
struct _shamuneko_module;

typedef struct _shamuneko_request shamuneko_request_t;
struct _shamuneko_request;

struct shamuneko_trending_result
{
	char const *id, *name, *description, *cover;
};

struct shamuneko_pages_result
{
	char const *img_url;

	char *img;
	size_t img_size;
};

typedef void (*download_pages_callback_t)(struct shamuneko_pages_result *results,
                                          size_t len,
                                          void *data);

typedef void (*get_trending_callback_t)(struct shamuneko_trending_result *results,
                                        size_t len,
                                        void *data);
typedef void (*get_pages_callback_t)(struct shamuneko_pages_result *results,
                                     size_t len,
                                     void *data);

struct shamuneko_http_funcs
{
	void*  (*create_session)();
	int    (*request)(void *data, char const *url, shamuneko_request_t *shamuneko_internal);
	void   (*destroy_session)(void *data);
};

#define SHAMUNEKO_HTTP_NOOP ((struct shamuneko_http_funcs){ 0 })

#endif // _SHAMUNEKO_TYPES_H_
