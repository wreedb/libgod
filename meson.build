project(
  'libgod',
  'cpp',
  version: files('version'),
  license: 'GPL-3.0-or-later',
  license_files: 'LICENSE.md',
  meson_version: '>= 1.3.0',
  default_options: ['cpp_std=c++23'],
)

fs = import('fs')

dependencies = []

libgod_args = ['-DBUILDING_LIBGOD']

libgod_sources = files(
  'src/god.cpp',
  'src/god/input.cpp',
  'src/god/token.cpp'
)

libgod_headers = 'include'

libgod = library(
  'god',
  libgod_sources,
  install: true,
  include_directories: [libgod_headers],
  cpp_shared_args: libgod_args,
  dependencies: dependencies,
)

libgod_dep = declare_dependency(
  include_directories: include_directories('include'),
  dependencies: dependencies,
  link_with: libgod,
)

meson.override_dependency('libgod', libgod_dep)

install_headers('include/god.hpp')

pkgconf = import('pkgconfig')

pkgconf.generate(libgod, description: 'A parsing library for GOD', subdirs: 'libgod')

test_config = configuration_data()

test_config.set_quoted('PROJECT_ROOT', meson.project_source_root())
test_config.set_quoted('SAMPLE_FILE', meson.project_source_root() / 'example' / 'sample.god')

configure_file(output: 'god-test.hpp', configuration: test_config)

test(
  'token',
  executable(
    'token',
    'test/token.cpp',
    include_directories: [libgod_headers],
    dependencies: dependencies,
    link_with: libgod))

test(
  'input-file',
  executable(
    'input-file',
    'test/input-file.cpp',
    include_directories: [libgod_headers],
    dependencies: dependencies,
    link_with: libgod))