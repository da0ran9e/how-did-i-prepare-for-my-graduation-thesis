build_lib_example(
  NAME wsn-cellular-tictoc-example
  SOURCE_FILES wsncellular-tictoc-example.cc
  LIBRARIES_TO_LINK
    ${libinternet}
    ${libmobility}
    ${libwifi}
    ${libapplications}
)
