build_lib(
  LIBNAME wsn
  SOURCE_FILES
    model/ini-parser.cc
    model/objects/resource-manager.cc
    model/objects/sensor-network.cc
    model/objects/wireless-channel.cc
    model/objects/wsn-app.cc
    model/objects/wsn-mac.cc
    model/objects/wsn-mobility.cc
    model/objects/wsn-node.cc
    model/objects/wsn-object-factory.cc
    model/objects/wsn-object-registry.cc
    model/objects/wsn-object.cc
    model/objects/wsn-routing.cc
    model/wsn-scenario.cc
    model/wsn-trace.cc
    
  HEADER_FILES
    model/ini-parser.h
    model/objects/resource-manager.h
    model/objects/sensor-network.h
    model/objects/wireless-channel.h
    model/objects/wsn-app.h
    model/objects/wsn-mac.h
    model/objects/wsn-mobility.h
    model/objects/wsn-node.h
    model/objects/wsn-object-factory.h
    model/objects/wsn-object-registry.h
    model/objects/wsn-object.h
    model/objects/wsn-routing.h
    model/wsn-scenario.h
    model/wsn-trace.h

  LIBRARIES_TO_LINK
    ${libcore}
)
