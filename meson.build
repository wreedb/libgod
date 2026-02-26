project('libgod', 'cpp',
    version: files('.version'),
    license: 'LGPL-3.0-or-later',
    meson_version: '>= 1.9.0',
    default_options: {
        'cpp_std': 'c++26'
    }
)

deps = []

version = meson.project_version()
semver = version.split('.')

version_major = semver[0]
version_minor = semver[1]
version_patch = semver[2]

subdir('include/god')

cxx = meson.get_compiler('cpp')

feat_tests = get_option('tests')
opt_libcxx = get_option('libcxx')
opt_stdstatic = get_option('static-stdlib')

if cxx.get_id() == 'clang' and opt_libcxx
    add_project_arguments('-Wno-unused-command-line-argument', language: 'cpp')
    add_project_arguments('-stdlib=libc++', language: 'cpp')
    add_project_link_arguments('-stdlib=libc++', language: 'cpp')
    deps += cxx.find_library('c++', required: true, static: opt_stdstatic)
    deps += cxx.find_library('c++abi', required: true, static: opt_stdstatic)
endif

if opt_stdstatic and cxx.get_id() == 'gcc'
    add_project_link_arguments('-static-libgcc', '-static-libstdc++', language: 'cpp')
endif


sources = files(
    'src/input.cpp',
    'src/output.cpp',
    'src/parse.cpp',
    'src/scanner.cpp',
    'src/token.cpp',
    'src/types.cpp',
    'src/util.cpp'
)

includes = include_directories('include')

lib = library(
    'god',
    sources,
    version: version,
    soversion: version_minor,
    include_directories: includes,
    dependencies: deps,
    install: get_option('install').allowed())

libgod_dep = declare_dependency(
    include_directories: includes,
    dependencies: deps,
    link_with: lib)

meson.override_dependency('libgod', libgod_dep)

headers = files(
    'include/god/base.hpp',
    'include/god/input.hpp',
    'include/god/output.hpp',
    'include/god/parse.hpp',
    'include/god/scanner.hpp',
    'include/god/token.hpp',
    'include/god/types.hpp',
    'include/god/util.hpp',
)

install_headers('include/god.hpp')
install_headers(headers, install_dir: get_option('includedir') / 'god')

if feat_tests.allowed()
    subdir('test')
endif


if get_option('install').allowed()

    conf = configuration_data()
    conf.set('PREFIX', get_option('prefix'))
    conf.set('VERSION', meson.project_version())

    configure_file(
        input: 'misc/libgod.pc.in',
        output: 'libgod.pc',
        configuration: conf,
        install: true,
        install_dir: get_option('prefix') / 'lib' / 'pkgconfig')

endif

if get_option('demos')

    executable(
        'demo',
        'misc/demo.cpp',
        include_directories: includes,
        link_with: lib,
        install: false)
    
    executable(
        'tokens',
        'misc/tokens.cpp',
        include_directories: includes,
        link_with: lib,
        install: false)

endif