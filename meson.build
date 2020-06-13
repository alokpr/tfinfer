project('tfinfer', 'c',
  version: '0.1'
)

glib_dep = dependency('glib-2.0')
tf_dep = meson.get_compiler('c').find_library('tensorflow')

sources = [
  'main.c',
  'tf_api.c'
]
executable('tfinfer',
  sources,
  dependencies: [glib_dep, tf_dep],
  install: true
)

data_dir = join_paths(get_option('datadir'), meson.project_name())
install_subdir('data', install_dir: data_dir)
