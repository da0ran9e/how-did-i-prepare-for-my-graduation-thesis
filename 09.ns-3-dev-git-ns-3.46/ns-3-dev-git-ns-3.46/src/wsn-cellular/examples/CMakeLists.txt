build_lib_example(
  NAME wsn-cellular-tictoc-example
  SOURCE_FILES wsncellular-tictoc-example.cc
  LIBRARIES_TO_LINK
    ${libwsn-cellular}
    ${libinternet}
    ${libmobility}
    ${libwifi}
    ${libapplications}
)
