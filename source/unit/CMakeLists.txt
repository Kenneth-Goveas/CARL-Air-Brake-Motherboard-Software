function(app_add name)
  add_executable(${name})
  target_sources(${name} PRIVATE source/${name}.cpp)
endfunction()

function(app_cmp_def name)
  target_compile_definitions(${name} PRIVATE ${ARGN})
endfunction()

function(app_cmp_opt name)
  target_compile_options(${name} PRIVATE ${ARGN})
endfunction()

function(app_lnk_tgt name)
  target_link_libraries(${name} PRIVATE ${ARGN})
endfunction()

function(app_lnk_opt name)
  target_link_options(${name} PRIVATE ${ARGN})
endfunction()
