build_lib_example(
  NAME wsn-routing-test-example
  SOURCE_FILES wsn-routing-test-example.cc
  LIBRARIES_TO_LINK
    ${libcore}
    ${libwsn-routing-test}
)
