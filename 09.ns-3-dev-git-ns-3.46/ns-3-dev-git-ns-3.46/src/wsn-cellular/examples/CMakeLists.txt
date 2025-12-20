build_lib_example(
  NAME wsn-cellular-tictoc-example
  SOURCE_FILES wsn-cellular-tictoc-example.cc
  LIBRARIES_TO_LINK
    ${libcore}
    ${libnetwork}
    ${libmobility}
    ${libspectrum}
    ${liblr-wpan}
    ${libwsn-cellular}
)
