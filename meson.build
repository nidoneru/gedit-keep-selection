shared_module(
  'keepselection',
  sources: files(
    'gedit-keep-selection-plugin.c',
    'gedit-keep-selection-app-activatable.c',
    'gedit-keep-selection-view-activatable.c',
    'gedit-keep-selection-window-activatable.c',
  ),
  include_directories: root_include_dir,
  dependencies: libgedit_dep,
  install: true,
  install_dir: get_option('libdir') / 'gedit/plugins',
  name_suffix: module_suffix,
)

custom_target(
  'keepselection.plugin',
  input: 'keepselection.plugin.desktop.in',
  output: 'keepselection.plugin',
  command: msgfmt_plugin_cmd,
  install: true,
  install_dir: get_option('libdir') / 'gedit/plugins',
)
