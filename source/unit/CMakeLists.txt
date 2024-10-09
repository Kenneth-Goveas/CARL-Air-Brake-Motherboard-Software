function(app name)
  add_executable(${name})
  target_sources(${name} PRIVATE source/${name}.cpp)
endfunction()

function(app_def name)
  target_compile_definitions(${name} PRIVATE ${ARGN})
endfunction()

function(app_lnk name)
  target_link_libraries(${name} PRIVATE ${ARGN})
endfunction()
