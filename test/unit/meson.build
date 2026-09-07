tests = [
    [ 'test_init' ],
]

foreach t : tests
    tt = executable(t[0],
        t[0] + '.c',
        link_with: libshamuneko,
        override_options: 'b_ndebug=false',
		c_args: '-DTESTS_DIR="' + meson.current_source_dir() + '/"',
        include_directories: libshamuneko_include)
    test(t.length() > 1 ? t[1] : t[0], tt)
endforeach
