build_lib(
  LIBNAME wsn
  SOURCE_FILES
    model/wsn-cellular-forwarder.cc
    model/wsn-cellular-header.cc
    model/wsn-cellular-routing.cc
  HEADER_FILES
    model/wsn-cellular-forwarder.h
    model/wsn-cellular-header.h
    model/wsn-cellular-routing.h
    
  LIBRARIES_TO_LINK
    ${libcore}
    ${libinternet}
    ${libmobility}
    ${libwifi}
    ${libapplications}
)
