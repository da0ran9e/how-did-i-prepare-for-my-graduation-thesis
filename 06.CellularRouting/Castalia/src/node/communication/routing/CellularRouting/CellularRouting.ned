package node.communication.routing.cellularRouting;

simple CellularRouting like node.communication.routing.iRouting {

    parameters:
        bool collectTraceInfo = default(false);
        int maxNetFrameSize = default(0);         
        int netDataFrameOverhead = default(12);   
        int netBufferSize = default(32);          

        double helloInterval = default(10s);      
        double cellRadius = default(50m);       
        bool isCH = default(false);              
        
        @display("i=block/routing");

    gates:

        input fromApplicationModule;
        output toApplicationModule;
        input fromMacModule;
        output toMacModule;
}
