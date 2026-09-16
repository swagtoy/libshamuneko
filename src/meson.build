sources = files('''
    shamuneko.c
    shamuneko_module.c
    http.c
    html.c
    json.c
'''.split())

libshamuneko_include = include_directories('.')

libshamuneko = static_library('shamuneko',
    sources,
    dependencies: [ lua_dep, libxml2_dep, cjson_dep ])
