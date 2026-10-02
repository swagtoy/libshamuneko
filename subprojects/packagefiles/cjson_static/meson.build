project('cjson_static', 'c')

cjson_sources = '''
cJSON.c
'''.split()

cjson_lib = static_library('cjson_static',
    cjson_sources,
    gnu_symbol_visibility: 'hidden',
    dependencies: [meson.get_compiler('c').find_library('m')]
)

cjson_dep = declare_dependency(
    link_with: cjson_lib,
    include_directories: include_directories('.'),
)
