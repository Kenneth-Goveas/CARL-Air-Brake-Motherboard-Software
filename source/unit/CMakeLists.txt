function(unit_add name)
  add_executable(${name})
  target_sources(${name} PRIVATE source/${name}.cpp)
endfunction()

function(unit_link_libs name)
  target_link_libraries(${name} PRIVATE ${ARGN})
endfunction()

function(unit_link_opts name)
  target_link_options(${name} PRIVATE ${ARGN})
endfunction()

function(unit_comp_defs name)
  target_compile_definitions(${name} PRIVATE ${ARGN})
endfunction()

function(unit_comp_opts name)
  target_compile_options(${name} PRIVATE ${ARGN})
endfunction()

unit_add(memory)
unit_link_libs(memory sys_logging sys_option ipc_channel)

unit_add(dashboard)
unit_link_libs(dashboard math_linalg sys_logging sys_option sys_interrupt ui_terminal ui_panel ipc_channel)

unit_add(streamer)
unit_link_libs(streamer math_linalg sys_logging sys_config sys_option sys_interrupt sys_timing ui_terminal ui_panel ipc_channel)

unit_add(recorder)
unit_link_libs(recorder math_linalg sys_logging sys_config sys_option sys_interrupt sys_timing ipc_channel dat_csvwrite)

unit_add(detector)
unit_link_libs(detector math_linalg sys_logging sys_config sys_interrupt sys_timing ipc_channel)

unit_add(calibrator)
unit_link_libs(calibrator math_linalg sys_logging sys_config sys_interrupt sys_timing ui_terminal ui_console ui_panel dev_i2c dev_bno055)

unit_add(instrument)
unit_link_libs(instrument math_linalg sys_logging sys_config sys_interrupt sys_timing ipc_channel dev_i2c dev_bmp388 dev_bno055)

unit_add(actuator)
unit_link_libs(actuator sys_logging sys_config sys_interrupt sys_timing ipc_channel dev_i2c dev_servo)

unit_add(simulator)
unit_link_libs(simulator math_linalg sys_logging sys_config sys_interrupt sys_timing ipc_channel dyn_simul)

unit_add(controller)
unit_link_libs(controller math_linalg sys_logging sys_config sys_interrupt sys_timing ipc_channel dyn_cntrl)

unit_add(estimator)
unit_link_libs(estimator math_linalg sys_logging sys_config sys_interrupt sys_timing ipc_channel dyn_estim)
