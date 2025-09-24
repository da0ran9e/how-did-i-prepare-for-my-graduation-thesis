package node.communication.routing.leachCmf;

simple LEACHCMF like node.communication.routing.iRouting {

    parameters:
        bool collectTraceInfo = default(false);
        int maxNetFrameSize = default(0);         
        int netDataFrameOverhead = default(12);   
        int netBufferSize = default(32);          
 
        double cellRadius = default(80);       
        bool isCH = default(false);   
        bool isSink = default(false);           
        
        @display("i=block/routing");

    gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}
