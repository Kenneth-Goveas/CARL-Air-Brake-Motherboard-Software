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
