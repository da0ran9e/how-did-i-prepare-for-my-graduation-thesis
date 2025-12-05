set(gsl_libraries)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(${GSL_FOUND})
  set(gsl_libraries
      ${GSL_LIBRARIES}
  )
endif()

set(source_files
    model/ini-parser.cc
    model/wsn-scenario.cc
)

set(header_files
    model/ini-parser.h
    model/wsn-scenario.h
)

build_lib(
  LIBNAME wsn
  SOURCE_FILES ${source_files}
  HEADER_FILES ${header_files}
  LIBRARIES_TO_LINK
    ${core}
  TEST_SOURCES
    
  GENERATE_EXPORT_HEADER
)

build_exec(
  EXECNAME wsn-sim
  SOURCE_FILES examples/wsn-sim.cc
  LIBRARIES_TO_LINK wsn
)