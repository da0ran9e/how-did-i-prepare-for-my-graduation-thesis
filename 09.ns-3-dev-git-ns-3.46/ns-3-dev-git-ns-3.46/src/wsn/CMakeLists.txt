build_lib(
  LIBNAME wsn
  SOURCE_FILES
    model/ini-parser.cc
    model/wsn-scenario.cc
    model/wsn-trace.cc
  HEADER_FILES
    model/ini-parser.h
    model/wsn-scenario.h
    model/wsn-trace.h
  LIBRARIES_TO_LINK
    ${libcore}
    ${libnetwork}
    ${libmobility}
    ${libspectrum}
    ${liblr-wpan}
)
