sources = files('''
    shamuneko.c
    shamuneko_module.c
    http.c
'''.split())

libshamuneko_include = include_directories('.')

libshamuneko = static_library('shamuneko',
    sources,
    dependencies: [ lua_dep ])
