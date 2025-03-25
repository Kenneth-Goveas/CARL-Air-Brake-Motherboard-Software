function(lib_add libname dirname filname)
  add_library(${libname} STATIC)
  target_include_directories(${libname} PUBLIC include template)
  target_sources(${libname} PRIVATE source/${dirname}/${filname}.cpp)
endfunction()

function(lib_link_libs libname)
  target_link_libraries(${libname} INTERFACE ${ARGN})
endfunction()

function(lib_link_opts libname)
  target_link_options(${libname} INTERFACE ${ARGN})
endfunction()

function(lib_comp_defs libname)
  target_compile_definitions(${libname} PRIVATE ${ARGN})
endfunction()

function(lib_comp_opts libname)
  target_compile_options(${libname} PRIVATE ${ARGN})
endfunction()

lib_add(math_lapack math lapack)
lib_link_libs(math_lapack blas lapack)

lib_add(math_linalg math linalg)
lib_link_libs(math_linalg m math_lapack)
lib_comp_defs(math_linalg _POSIX_C_SOURCE=200112L)

lib_add(math_randnum math randnum)
lib_link_libs(math_randnum m math_linalg)

lib_add(sys_logging sys logging)
lib_link_libs(sys_logging math_linalg)

lib_add(sys_config sys config)
lib_link_libs(sys_config math_linalg sys_logging)

lib_add(sys_interrupt sys interrupt)
lib_link_libs(sys_interrupt sys_logging)

lib_add(sys_timing sys timing)
lib_link_libs(sys_timing sys_logging)

lib_add(ui_terminal ui terminal)
lib_link_libs(ui_terminal sys_logging)

lib_add(ui_input ui input)
lib_link_libs(ui_input math_linalg sys_logging)

lib_add(ui_output ui output)
lib_link_libs(ui_output math_linalg sys_logging)

lib_add(ui_console ui console)
lib_link_libs(ui_console math_linalg sys_logging ui_terminal ui_input ui_output)

lib_add(ui_panel ui panel)
lib_link_libs(ui_panel math_linalg sys_logging ui_terminal ui_input ui_output)

lib_add(ipc_semaphore ipc semaphore)
lib_link_libs(ipc_semaphore sys_logging)
lib_link_opts(ipc_semaphore -pthread)

lib_add(ipc_sharedmem ipc sharedmem)
lib_link_libs(ipc_sharedmem rt math_linalg sys_logging)
lib_comp_defs(ipc_sharedmem _POSIX_C_SOURCE=200112L)

lib_add(ipc_channel ipc channel)
lib_link_libs(ipc_channel math_linalg sys_logging ipc_semaphore ipc_sharedmem)

lib_add(dev_i2c dev i2c)
lib_link_libs(dev_i2c sys_logging)

lib_add(dev_servo dev servo)
lib_link_libs(dev_servo sys_logging dev_i2c)

lib_add(dev_bmp388 dev bmp388)
lib_link_libs(dev_bmp388 m sys_logging dev_i2c)

lib_add(dev_bno055 dev bno055)
lib_link_libs(dev_bno055 math_linalg sys_logging dev_i2c)

lib_add(dat_csvread dat csvread)
lib_link_libs(dat_csvread sys_logging)

lib_add(dat_csvwrite dat csvwrite)
lib_link_libs(dat_csvwrite sys_logging)

lib_add(dyn_simul dyn simul)
lib_link_libs(dyn_simul m math_linalg math_randnum sys_logging dat_csvread)

lib_add(dyn_cntrl dyn cntrl)
lib_link_libs(dyn_cntrl m math_linalg sys_logging dat_csvread)

lib_add(dyn_estim dyn estim)
lib_link_libs(dyn_estim m math_linalg sys_logging dat_csvread)
lib_comp_defs(dyn_estim _POSIX_C_SOURCE=200112L)
