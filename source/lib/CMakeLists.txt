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

lib(sys_logging sys logging)
