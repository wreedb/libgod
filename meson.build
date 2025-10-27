project(
  'libgod',
  'cpp',
  version: files('version'),
  license: 'GPL-3.0-or-later',
  license_files: 'LICENSE.md',
  meson_version: '>= 1.3.0',
  default_options: ['cpp_std=c++23'],
)

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

libgod_test = executable(
  'god_test',
  'test/god.cpp',
  include_directories: [libgod_headers],
  dependencies: dependencies,
  link_with: libgod,
)

t_token = executable('god_token', 'test/token.cpp', include_directories: [libgod_headers], dependencies: dependencies, link_with: libgod)

test('god', libgod_test)
test('token', t_token)

libgod_dep = declare_dependency(
  include_directories: include_directories('include'),
  dependencies: dependencies,
  link_with: libgod,
)

meson.override_dependency('libgod', libgod_dep)

install_headers('include/god.hpp')

pkgconf = import('pkgconfig')

pkgconf.generate(libgod, description: 'A parsing library for GOD', subdirs: 'libgod')
