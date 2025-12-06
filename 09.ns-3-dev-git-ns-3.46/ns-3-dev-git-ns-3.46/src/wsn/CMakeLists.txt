build_lib(
  LIBNAME wsn
  SOURCE_FILES
    model/ini-parser.cc
    model/wsn-scenario.cc
  HEADER_FILES
    model/ini-parser.h
    model/wsn-scenario.h
  LIBRARIES_TO_LINK
    ${libcore}
)
