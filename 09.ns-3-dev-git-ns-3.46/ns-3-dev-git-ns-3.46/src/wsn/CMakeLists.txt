build_lib(
  LIBNAME wsn
  SOURCE_FILES
    model/ini-parser.cc
    model/resource-manager.cc
    model/sensor-network.cc
    model/wireless-channel.cc
    model/wsn-app.cc
    model/wsn-mac.cc
    model/wsn-mobility.cc
    model/wsn-node.cc
    model/wsn-object-factory.cc
    model/wsn-object-registry.cc
    model/wsn-object.cc
    model/wsn-routing.cc
    model/wsn-scenario.cc
    model/wsn-trace.cc
    
  HEADER_FILES
    model/ini-parser.h
    model/resource-manager.h
    model/sensor-network.h
    model/wireless-channel.h
    model/wsn-app.h
    model/wsn-mac.h
    model/wsn-mobility.h
    model/wsn-node.h
    model/wsn-object-factory.h
    model/wsn-object-registry.h
    model/wsn-object.h
    model/wsn-routing.h
    model/wsn-scenario.h
    model/wsn-trace.h

  LIBRARIES_TO_LINK
    ${libcore}
)
