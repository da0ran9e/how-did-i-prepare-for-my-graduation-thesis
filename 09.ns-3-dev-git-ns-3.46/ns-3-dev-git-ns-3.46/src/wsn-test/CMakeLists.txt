set(gsl_libraries)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(${GSL_FOUND})
  set(gsl_libraries
      ${GSL_LIBRARIES}
  )
endif()

set(source_files
    model/wsn-test-model.cc
)

set(header_files
    model/wsn-test-model.h
)

build_lib(
  LIBNAME wsn-test
  SOURCE_FILES ${source_files}
  HEADER_FILES ${header_files}
  LIBRARIES_TO_LINK
    ${core}
  TEST_SOURCES
    
  GENERATE_EXPORT_HEADER
)

build_exec(
  EXECNAME wsn-sim-test
  SOURCE_FILES examples/wsn-sim-test.cc
  LIBRARIES_TO_LINK wsn-test
)