build_lib_example(
  NAME wsn-sim
  SOURCE_FILES wsn-sim.cc
  LIBRARIES_TO_LINK
    ${libcore}
    ${libwsn}
)
