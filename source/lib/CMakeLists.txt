function(lib_add dirname filname)
  add_library(${dirname}_${filname} STATIC)
  target_include_directories(${dirname}_${filname} PUBLIC include template)
  target_sources(${dirname}_${filname} PRIVATE source/${dirname}/${filname}.cpp)
endfunction()

function(lib_comp_defs libname)
  target_compile_definitions(${libname} PRIVATE ${ARGN})
endfunction()

function(lib_comp_opts libname)
  target_compile_options(${libname} PRIVATE ${ARGN})
endfunction()

function(lib_link_libs libname)
  target_link_libraries(${libname} INTERFACE ${ARGN})
endfunction()

function(lib_link_opts libname)
  target_link_options(${libname} INTERFACE ${ARGN})
endfunction()

lib_add(math lapack)
lib_link_libs(math_lapack blas lapack)

lib_add(math linalg)
lib_link_libs(math_linalg m math_lapack)

lib_add(math randnum)
lib_link_libs(math_randnum m math_linalg)

lib_add(sys logging)
lib_link_libs(sys_logging math_linalg)

lib_add(sys interrupt)
lib_link_libs(sys_interrupt sys_logging)

lib_add(sys timing)
lib_link_libs(sys_timing sys_logging)

lib_add(sys option)
lib_link_libs(sys_option sys_logging)

lib_add(sys config)
lib_link_libs(sys_config math_linalg sys_logging)

lib_add(ui terminal)
lib_link_libs(ui_terminal sys_logging)

lib_add(ui input)
lib_link_libs(ui_input math_linalg sys_logging)

lib_add(ui output)
lib_link_libs(ui_output math_linalg sys_logging)

lib_add(ui screen)
lib_link_libs(ui_screen sys_logging ui_terminal)

lib_add(ui panel)
lib_link_libs(ui_panel math_linalg sys_logging ui_terminal ui_input ui_output)
