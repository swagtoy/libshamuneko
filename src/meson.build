sources = files('''
    shamuneko.c
    shamuneko_module.c
    http.c
    html.c
    json.c
    returnfuncs.c
    util.c
'''.split())

headers = []
amalg_includes = []
foreach src : sources
    header = src.full_path().replace('.c', '.h')
    headers += header

    # With these headers, we'll generate a big set of commands to
    #  preprocess, that way we can exclude duplicate definitions (instead
    #  of calling `cpp' on each file, foer example)
    amalg_includes += [ '-include', header ]
endforeach

message(amalg_includes)

libshamuneko_include = include_directories('.')
libshamuneko_deps = [ lua_dep, libxml2_dep, cjson_dep, miniz_dep ]

libshamuneko = static_library('shamuneko',
    sources,
    dependencies: libshamuneko_deps)

libshamuneko_shared = shared_library('shamuneko',
    sources,
    dependencies: libshamuneko_deps)

# We build an almagamated header that we can easily include into luajit.
header_amalg = run_command(
    'cpp', [ '-P', '-E', '-std=c99', '-DSHAMUNEKO_AMALGAMATION_HEADER=plz', amalg_includes, '/dev/null' ],
    capture: true,
    check: true,
).stdout()

configure_file(
    output: 'shamuneko_amalgamation_header.h',
    capture: true,
    command: [ 'echo', header_amalg ]
)
