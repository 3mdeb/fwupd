cargs = ['-DG_LOG_DOMAIN="FuPluginMitigations"']

shared_module('fu_plugin_mitigations',
  fu_hash,
  sources : [
    'fu-plugin-mitigations.c',
  ],
  include_directories : [
    root_incdir,
    fwupd_incdir,
    fwupdplugin_incdir,
  ],
  install : true,
  install_dir: plugin_dir,
  c_args : [
    cargs,
  ],
  link_with : [
    fwupd,
    fwupdplugin,
  ],
  dependencies : [
    plugin_deps,
  ],
)
