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

if get_option('libxml2_static')
    libxml2_subp = subproject('libxml2', default_options: [
        'iconv=disabled', 'threads=disabled', 'regexps=disabled',
    ])
    xml_src_files = libxml2_subp.get_variable('xml_src_files')
    xml_includes = libxml2_subp.get_variable('config_dir')
    libxml2_lib = static_library('xml2',
        xml_src_files,
        dependencies: libxml2_subp.get_variable('xml_deps'),
        include_directories: xml_includes,
    )
    libxml2_dep = declare_dependency(
        link_with: libxml2_lib,
        include_directories: xml_includes,
    )
else
    #libxml2_dep = dependency('libxml-2.0', required: true)
endif

if get_option('cjson_static')
    cjson_subp = subproject('cjson_static')
    cjson_dep = cjson_subp.get_variable('cjson_dep')
else
    cjson_dep = dependency('libcjson', required: true)
endif

miniz_subp = subproject('miniz')
miniz_dep = miniz_subp.get_variable('miniz_dep')

subdir('src')
subdir('test/unit')
