function(lib_add libname dirname filname)
  add_library(${libname} STATIC)
  target_include_directories(${libname} PUBLIC include template)
  target_sources(${libname} PRIVATE source/${dirname}/${filname}.cpp)
endfunction()

function(lib_cmp_def libname)
  target_compile_definitions(${libname} PRIVATE ${ARGN})
endfunction()

function(lib_cmp_opt libname)
  target_compile_options(${libname} PRIVATE ${ARGN})
endfunction()

function(lib_lnk_tgt libname)
  target_link_libraries(${libname} INTERFACE ${ARGN})
endfunction()

function(lib_lnk_opt libname)
  target_link_options(${libname} INTERFACE ${ARGN})
endfunction()

lib_add(math_lapack math lapack)
lib_lnk_tgt(math_lapack blas lapack)

lib_add(math_linalg math linalg)
lib_cmp_def(math_linalg _POSIX_C_SOURCE=200112L)
lib_lnk_tgt(math_linalg m math_lapack)

lib_add(math_randn math randn)
lib_lnk_tgt(math_randn m math_linalg)

lib_add(sys_logging sys logging)

lib_add(sys_config sys config)
lib_lnk_tgt(sys_config math_linalg sys_logging)

lib_add(ui_terminal ui terminal)
lib_lnk_tgt(ui_terminal sys_logging)

lib_add(ui_input ui input)
lib_lnk_tgt(ui_input math_linalg sys_logging)

lib_add(ui_output ui output)
lib_lnk_tgt(ui_output math_linalg sys_logging)

lib_add(ui_panel ui panel)
lib_lnk_tgt(ui_panel math_linalg sys_logging ui_terminal ui_input ui_output)

lib_add(ui_console ui console)
lib_lnk_tgt(ui_console math_linalg sys_logging ui_terminal ui_input ui_output)

lib_add(ipc_sema ipc sema)
lib_lnk_tgt(ipc_sema sys_logging)
lib_lnk_opt(ipc_sema -pthread)

lib_add(ipc_shmem ipc shmem)
lib_cmp_def(ipc_shmem _POSIX_C_SOURCE=200112L)
lib_lnk_tgt(ipc_shmem rt math_linalg sys_logging)
