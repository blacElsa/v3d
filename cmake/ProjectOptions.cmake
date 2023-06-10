include(CMakeDependentOption)

macro(v3d_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR
      CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
     AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()
  
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR
      CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
     AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(v3d_setup_options)
  option(v3d_ENABLE_HARDENING "Enable hardening" ON)
  option(v3d_ENABLE_COVERAGE "Enable coverage" OFF)
  cmake_dependent_option(v3d_ENABLE_GLOBAL_HARDENING
                         "Attempt to push hardening option to built dependencies"
                         ON v3d_ENABLE_HARDENING OFF)

  v3d_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR v3d_PACKAGING_MAINTAINER_MODE)
    option(v3d_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(v3d_WARNINGS_AS_ERROR "Treat warnings as error" OFF)
    option(v3d_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(v3d_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(v3d_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(v3d_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(v3d_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(v3d_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(v3d_ENABLE_UNITY_BUILD "Enable unity build" OFF)
    option(v3d_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(v3d_ENABLE_CPPCHECK "Enable cppcheck" OFF)
    option(v3d_ENABLE_PCH "Enable PCH" OFF)
    option(v3d_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(v3d_ENABLE_IPO "Enable IPO/LTO" ON)
    option(v3d_WARNINGS_AS_ERROR "Treat warnings as error" ON)
    option(v3d_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(v3d_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(v3d_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(v3d_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(v3d_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(v3d_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(v3d_ENABLE_UNITY_BUILD "Enable unity build" OFF)
    option(v3d_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(v3d_ENABLE_CPPCHECK "Enable cppcheck" ON)
    option(v3d_ENABLE_PCH "Enable PCH" OFF)
    option(v3d_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if (NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      v3d_ENABLE_IPO
      v3d_WARNINGS_AS_ERROR
      v3d_ENABLE_USER_LINKER
      v3d_ENABLE_SANITIZER_ADDRESS
      v3d_ENABLE_SANITIZER_LEAK
      v3d_ENABLE_SANITIZER_UNDEFINED
      v3d_ENABLE_SANITIZER_THREAD
      v3d_ENABLE_SANITIZER_MEMORY
      v3d_ENABLE_UNITY_BUILD 
      v3d_ENABLE_CLANG_TIDY
      v3d_ENABLE_CPPCHECK
      v3d_ENABLE_PCH
      v3d_ENABLE_CACHE
      v3d_ENABLE_COVERAGE)
  endif()

  # maybe add fuzzers
endmacro()

macro(v3d_global_options)
  if(v3d_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    v3d_enable_ipo()
  endif()

  v3d_supports_sanitizers()

  if(v3d_ENABLE_HARDENING AND v3d_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR v3d_ENABLE_SANITIZER_ADDRESS
       OR v3d_ENABLE_SANITIZER_THREAD
       OR v3d_ENABLE_SANITIZER_UNDEFINED
       OR v3d_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${v3d_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${v3d_ENABLE_SANITIZER_UNDEFINED}")
    v3d_enable_hardening(v3d_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(v3d_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(v3d_warnings INTERFACE)
  add_library(v3d_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  v3d_set_project_warnings(v3d_warnings ${v3d_WARNINGS_AS_ERROR} "" "" "" "")

  if(v3d_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    v3d_configure_linker(v3d_options)
  endif()

  include(cmake/Sanitizers.cmake)
  v3d_enable_sanitizers(v3d_options
                        ${v3d_ENABLE_SANITIZER_ADDRESS}
                        ${v3d_ENABLE_SANITIZER_LEAK}
                        ${v3d_ENABLE_SANITIZER_UNDEFINED}
                        ${v3d_ENABLE_SANITIZER_THREAD}
                        ${v3d_ENABLE_SANITIZER_MEMORY})

  set_target_properties(v3d_options PROPERTIES UNITY_BUILD ${v3d_ENABLE_UNITY_BUILD})

  if(v3d_ENABLE_PCH)
    target_precompile_headers(v3d_options
                              INTERFACE
                              <vector>
                              <string>
                              <utility>)
  endif()

  if(v3d_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    v3d_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(v3d_ENABLE_CLANG_TIDY)
    v3d_enable_clang_tidy(v3d_options v3d_WARNINGS_AS_ERRORS)
  endif()

  if(v3d_ENABLE_CPPCHECK)
    v3d_enable_cppcheck(v3d_WARNINGS_AS_ERRORS "") # override cppcheck options
  endif()

  if(v3d_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    v3d_enable_coverage(v3d_options)
  endif()

  if(v3d_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(v3d_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(v3d_ENABLE_HARDENING AND NOT v3d_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR v3d_ENABLE_SANITIZER_UNDEFINED
       OR v3d_ENABLE_SANITIZER_ADDRESS
       OR v3d_ENABLE_SANITIZER_THREAD
       OR v3d_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    v3d_enable_hardening(v3d_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()