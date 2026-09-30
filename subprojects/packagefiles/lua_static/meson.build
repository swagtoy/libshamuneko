# This is maybe not a permanent solution? I wrote a hefty comment in the root meson.build
project('lua_static', 'c', version: '5.4.9')

lua_sources_raw = '''
lapi.c lauxlib.c lbaselib.c lcode.c lcorolib.c lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c lutf8lib.c lvm.c lzio.c
'''.split()

lua_prefix = 'src'
lua_sources = []
foreach file : lua_sources_raw
    lua_sources += lua_prefix / file
endforeach

lua_lib = static_library('lua_static',
    lua_sources,
    gnu_symbol_visibility: 'hidden',
    dependencies: [meson.get_compiler('c').find_library('m')]
)

lua_dep = declare_dependency(
    link_with: lua_lib,
    include_directories: include_directories('src'),
)
