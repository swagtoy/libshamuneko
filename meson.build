project('libshamuneko', ['c'])

# KOReader (as an example) uses luajit, however, since luajit is
#  loaded as a shared library, this actually ends up causing
#  issues where whenever our lua shared library loads (as
#  libshamuneko gets dlopen'd/ffi.open'd), we end up having
#  conflicting symbols in the global symbol namespace.
#
# A solution I would've preferred is to dlopen lua ourself (it'd be
#  some work though...), but for now it was a bit simpler to just link
#  lua itself since lua is pretty small. It increases our shared lib
#  to 1mb... who cares...
if get_option('lua_static')
    lua_subp = subproject('lua_static')
    lua_dep = lua_subp.get_variable('lua_dep')
else
    lua_dep = dependency('lua5.4', required: true)
endif
libxml2_dep = dependency('libxml-2.0', required: true)
cjson_dep = dependency('libcjson', required: true)

subdir('src')
subdir('test/unit')
