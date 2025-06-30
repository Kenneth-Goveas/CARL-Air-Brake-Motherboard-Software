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

lib_add(dat txtwrite)
lib_link_libs(dat_txtwrite math_linalg sys_logging)

lib_add(dat csvwrite)
lib_link_libs(dat_csvwrite sys_logging)

lib_add(dat csvread)
lib_link_libs(dat_csvread sys_logging)

lib_add(ipc semaphore)
lib_link_libs(ipc_semaphore sys_logging)
lib_link_opts(ipc_semaphore -pthread)

lib_add(ipc sharedmem)
lib_link_libs(ipc_sharedmem rt math_linalg sys_logging)
lib_comp_defs(ipc_sharedmem _POSIX_C_SOURCE=200112L)

lib_add(ipc channel)
lib_link_libs(ipc_channel math_linalg sys_logging ipc_semaphore ipc_sharedmem)

lib_add(dev i2c)
lib_link_libs(dev_i2c sys_logging)

lib_add(dev servo)
lib_link_libs(dev_servo sys_logging dev_i2c)

lib_add(dev bmp388)
lib_link_libs(dev_bmp388 m sys_logging dev_i2c)

lib_add(dev bno055)
lib_link_libs(dev_bno055 math_linalg sys_logging dev_i2c)

lib_add(dyn simul)
lib_link_libs(dyn_simul m math_linalg math_randnum sys_logging dat_csvread)
