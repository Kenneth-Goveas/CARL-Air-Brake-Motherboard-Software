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
