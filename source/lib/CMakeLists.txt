function(lib libname dirname filname)
  add_library(${libname} STATIC)
  target_include_directories(${libname} PUBLIC include template)
  target_sources(${libname} PRIVATE source/${dirname}/${filname}.cpp)
endfunction()

function(lib_def libname)
  target_compile_definitions(${libname} PRIVATE ${ARGN})
endfunction()

function(lib_lnk libname)
  target_link_libraries(${libname} INTERFACE ${ARGN})
endfunction()

lib(math_lapack math lapack)

lib(sys_logging sys logging)
lib(sys_config sys config)

lib_lnk(math_lapack blas lapack)

lib_lnk(sys_config sys_logging)
