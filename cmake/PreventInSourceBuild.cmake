function(v3d_assure_out_of_source_builds)
  get_filename_component(srcdir ${CMAKE_SOURCE_DIR} REALPATH)
  get_filename_component(bindir ${CMAKE_BINARY_DIR} REALPATH)

  if(srcdir STREQUAL bindir)
    message(FATAL_ERROR "Build in source directory disabled. Please create a separate build directory and run cmake from there")
  endif()
endfunction()

v3d_assure_out_of_source_builds()
