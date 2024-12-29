package node.communication.routing.bCastRouting;

simple BCastRouting like node.communication.routing.iRouting {
 parameters:
	bool collectTraceInfo = default (false);
	int nId = default(0);
	int maxNetFrameSize = default (0);					// bytes, 0 means no limit
	int netBufferSize = default (32);					// number of messages
	int netDataFrameOverhead = default (14);			// bytes
	int mpathRingsSetupFrameOverhead = default (13);	// bytes
	double netSetupTimeout = default (50.0);					// msec

 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

