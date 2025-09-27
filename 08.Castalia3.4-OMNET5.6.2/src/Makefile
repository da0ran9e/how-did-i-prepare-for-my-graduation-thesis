#
# OMNeT++/OMNEST Makefile for CastaliaBin
#
# This file was generated with the command:
#  opp_makemake -f --deep -o CastaliaBin -O out -I. -DAUTOIMPORT_OMNETPP_NAMESPACE
#

# Name of target to be created (-o option)
TARGET = CastaliaBin$(D)$(EXE_SUFFIX)
TARGET_DIR = .

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/helpStructures/CastaliaModule.o \
    $O/helpStructures/DebugInfoWriter.o \
    $O/helpStructures/TimerService.o \
    $O/node/application/VirtualApplication.o \
    $O/node/application/bridgeTest/BridgeTest.o \
    $O/node/application/connectivityMap/ConnectivityMap.o \
    $O/node/application/simpleAggregation/SimpleAggregation.o \
    $O/node/application/throughputTest/ThroughputTest.o \
    $O/node/application/valuePropagation/ValuePropagation.o \
    $O/node/application/valueReporting/ValueReporting.o \
    $O/node/communication/mac/VirtualMac.o \
    $O/node/communication/mac/baselineBanMac/BaselineBANMac.o \
    $O/node/communication/mac/bypassMac/BypassMAC.o \
    $O/node/communication/mac/mac802154/Basic802154.o \
    $O/node/communication/mac/mac802154/staticGTS802154/StaticGTS802154.o \
    $O/node/communication/mac/tMac/TMAC.o \
    $O/node/communication/mac/tunableMac/TunableMAC.o \
    $O/node/communication/radio/Radio.o \
    $O/node/communication/radio/RadioSupportFunctions.o \
    $O/node/communication/routing/VirtualRouting.o \
    $O/node/communication/routing/bypassRouting/BypassRouting.o \
    $O/node/communication/routing/cellularRouting/CellularRouting.o \
    $O/node/communication/routing/multipathRingsRouting/MultipathRingsRouting.o \
    $O/node/mobilityManager/VirtualMobilityManager.o \
    $O/node/mobilityManager/lineMobilityManager/LineMobilityManager.o \
    $O/node/mobilityManager/noMobilityManager/NoMobilityManager.o \
    $O/node/resourceManager/ResourceManager.o \
    $O/node/sensorManager/SensorManager.o \
    $O/physicalProcess/carsPhysicalProcess/CarsPhysicalProcess.o \
    $O/physicalProcess/customizablePhysicalProcess/CustomizablePhysicalProcess.o \
    $O/wirelessChannel/defaultChannel/WirelessChannel.o \
    $O/wirelessChannel/defaultChannel/WirelessChannelTemporal.o \
    $O/wirelessChannel/traceChannel/TraceChannel.o \
    $O/helpStructures/TimerServiceMessage_m.o \
    $O/node/application/ApplicationPacket_m.o \
    $O/node/application/valueReporting/ValueReportingPacket_m.o \
    $O/node/communication/mac/MacPacket_m.o \
    $O/node/communication/mac/baselineBanMac/BaselineMacPacket_m.o \
    $O/node/communication/mac/mac802154/Basic802154Packet_m.o \
    $O/node/communication/mac/tMac/TMacPacket_m.o \
    $O/node/communication/mac/tunableMac/TunableMacControl_m.o \
    $O/node/communication/mac/tunableMac/TunableMacPacket_m.o \
    $O/node/communication/radio/RadioControlMessage_m.o \
    $O/node/communication/routing/RoutingPacket_m.o \
    $O/node/communication/routing/bypassRouting/BypassRoutingPacket_m.o \
    $O/node/communication/routing/cellularRouting/CellularRoutingPacket_m.o \
    $O/node/communication/routing/multipathRingsRouting/MultipathRingsRoutingControl_m.o \
    $O/node/communication/routing/multipathRingsRouting/MultipathRingsRoutingPacket_m.o \
    $O/node/mobilityManager/MobilityManagerMessage_m.o \
    $O/node/resourceManager/ResourceManagerMessage_m.o \
    $O/node/sensorManager/SensorManagerMessage_m.o \
    $O/physicalProcess/PhysicalProcessMessage_m.o \
    $O/wirelessChannel/WirelessChannelMessages_m.o

# Message files
MSGFILES = \
    helpStructures/TimerServiceMessage.msg \
    node/application/ApplicationPacket.msg \
    node/application/valueReporting/ValueReportingPacket.msg \
    node/communication/mac/MacPacket.msg \
    node/communication/mac/baselineBanMac/BaselineMacPacket.msg \
    node/communication/mac/mac802154/Basic802154Packet.msg \
    node/communication/mac/tMac/TMacPacket.msg \
    node/communication/mac/tunableMac/TunableMacControl.msg \
    node/communication/mac/tunableMac/TunableMacPacket.msg \
    node/communication/radio/RadioControlMessage.msg \
    node/communication/routing/RoutingPacket.msg \
    node/communication/routing/bypassRouting/BypassRoutingPacket.msg \
    node/communication/routing/cellularRouting/CellularRoutingPacket.msg \
    node/communication/routing/multipathRingsRouting/MultipathRingsRoutingControl.msg \
    node/communication/routing/multipathRingsRouting/MultipathRingsRoutingPacket.msg \
    node/mobilityManager/MobilityManagerMessage.msg \
    node/resourceManager/ResourceManagerMessage.msg \
    node/sensorManager/SensorManagerMessage.msg \
    physicalProcess/PhysicalProcessMessage.msg \
    wirelessChannel/WirelessChannelMessages.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DAUTOIMPORT_OMNETPP_NAMESPACE $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
#
# Use the older message compiler. New compiler was introduced in OMNeT++ 5.3
#
MSGC:=$(MSGC) --msg4

# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
