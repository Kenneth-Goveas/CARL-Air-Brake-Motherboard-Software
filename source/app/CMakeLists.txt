function(app_add name)
  add_executable(${name})
  target_sources(${name} PRIVATE source/${name}.cpp)
endfunction()

function(app_comp_defs name)
  target_compile_definitions(${name} PRIVATE ${ARGN})
endfunction()

function(app_comp_opts name)
  target_compile_options(${name} PRIVATE ${ARGN})
endfunction()

function(app_link_libs name)
  target_link_libraries(${name} PRIVATE ${ARGN})
endfunction()

function(app_link_opts name)
  target_link_options(${name} PRIVATE ${ARGN})
endfunction()

app_add(memory)
app_link_libs(memory
  sys_logging sys_option
  ipc_channel
)

app_add(dashboard)
app_link_libs(dashboard
  math_linalg
  sys_logging sys_interrupt sys_option
  ui_terminal ui_input ui_output ui_screen ui_panel
  ipc_channel
)

app_add(streamer)
app_link_libs(streamer
  math_linalg
  sys_logging sys_interrupt sys_timing sys_option sys_config
  ui_terminal ui_input ui_output ui_screen ui_panel
  ipc_channel
)

app_add(recorder)
app_link_libs(recorder
  math_linalg
  sys_logging sys_interrupt sys_timing sys_option sys_config
  dat_csvwrite
  ipc_channel
)
