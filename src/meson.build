sources = files('''
    shamuneko.c
    shamuneko_module.c
    http.c
    html.c
    json.c
    returnfuncs.c
    util.c
'''.split())

libshamuneko_include = include_directories('.')
libshamuneko_deps = [ lua_dep, libxml2_dep, cjson_dep ]

libshamuneko = static_library('shamuneko',
    sources,
    dependencies: libshamuneko_deps)

libshamuneko_shared = shared_library('shamuneko',
    sources,
    dependencies: libshamuneko_deps)
