project('tfinfer', 'c',
  version: '0.1'
)

tf_dep = meson.get_compiler('c').find_library('tensorflow')

sources = [
  'main.c'
]
executable('tfinfer',
  sources,
  dependencies: [tf_dep],
  install: true
)
