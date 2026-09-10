tests = [
    [ 'test_init' ],
    [ 'test_curl', [ dependency('libcurl')] ],
]

foreach t : tests
    tt = executable(t[0],
        t[0] + '.c',
        link_with: libshamuneko,
        dependencies: (t.length() > 1) ? t[1] : [],
        override_options: 'b_ndebug=false',
		c_args: '-DTESTS_DIR="' + meson.current_source_dir() + '/"',
        include_directories: libshamuneko_include)
    test(t[0], tt)
endforeach
