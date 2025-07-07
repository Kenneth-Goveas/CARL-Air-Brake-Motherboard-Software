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

app_add(detector)
app_link_libs(detector
  math_linalg
  sys_logging sys_interrupt sys_timing sys_config
  ipc_channel
)

app_add(calibrator)
app_link_libs(calibrator
  math_linalg
  sys_logging sys_interrupt sys_timing sys_config
  ui_terminal ui_input ui_output ui_screen ui_panel
  dat_txtwrite
  dev_i2c dev_bno055
)

app_add(instrument)
app_link_libs(instrument
  math_linalg
  sys_logging sys_interrupt sys_timing sys_config
  ipc_channel
  dev_i2c dev_bmp388 dev_bno055
)

app_add(actuator)
app_link_libs(actuator
  sys_logging sys_interrupt sys_timing sys_config
  ipc_channel
  dev_i2c dev_servo
)

app_add(simulator)
app_link_libs(simulator
  math_linalg
  sys_logging sys_interrupt sys_timing sys_config
  ipc_channel
  dyn_simul
)

app_add(controller)
app_link_libs(controller
  math_linalg
  sys_logging sys_interrupt sys_timing sys_config
  ipc_channel
  dyn_cntrl
)

app_add(estimator)
app_link_libs(estimator
  math_linalg
  sys_logging sys_interrupt sys_timing sys_config
  ipc_channel
  dyn_estim
)
