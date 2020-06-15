project('tfinfer', 'c',
  version: '0.1'
)

data_dir = join_paths(get_option('datadir'), meson.project_name())
install_data('data/model.pb', install_dir: data_dir)

cdata = configuration_data()
cdata.set_quoted('TF_MODEL_FILE',
  join_paths(get_option('prefix'), data_dir, 'model.pb'))
configure_file(output: 'config.h', configuration: cdata)

glib_dep = dependency('glib-2.0')
tf_dep = meson.get_compiler('c').find_library('tensorflow')

sources = [
  'tf_api.c',
  'tf_infer.c',
]
executable('tf_infer',
  sources,
  dependencies: [glib_dep, tf_dep],
  install: true
)
