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
