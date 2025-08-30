project(
  'Tappr',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++14'],
)

raylib_dep = dependency('raylib', required: true)
dependencies = [
  raylib_dep,
]

includeDirs = [
  include_directories('include'),
]

src = [
  'src/main.cpp',
  'src/objects.cpp',
  'src/game.cpp',
]

exe = executable(
  'Tappr',
  src,
  install : true,
  dependencies : dependencies,
  include_directories : includeDirs,
)

test('basic', exe)
