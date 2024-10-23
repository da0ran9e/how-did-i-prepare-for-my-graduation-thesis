simple Node
{
    parameters:
        @display("i=abstract/router");
    gates:
        inout gate[];
}

network GraphNetwork
{
    parameters:
        int nodeCount = 20; // Number of nodes
        @display("bgb=2500,1500"); // Background size of the canvas
    submodules:
        node0: Node { @display("p=100,200;i=abstract/router"); }
        node1: Node { @display("p=300,250;i=abstract/router"); }
        node2: Node { @display("p=500,400;i=abstract/router"); }
        node3: Node { @display("p=700,450;i=abstract/router"); }
        node4: Node { @display("p=900,200;i=abstract/router"); }
        node5: Node { @display("p=200,600;i=abstract/router"); }
        node6: Node { @display("p=400,700;i=abstract/router"); }
        node7: Node { @display("p=600,800;i=abstract/router"); }
        node8: Node { @display("p=800,600;i=abstract/router"); }
        node9: Node { @display("p=1000,450;i=abstract/router"); }
        node10: Node { @display("p=1200,300;i=abstract/router"); }
        node11: Node { @display("p=1400,500;i=abstract/router"); }
        node12: Node { @display("p=1600,600;i=abstract/router"); }
        node13: Node { @display("p=1800,700;i=abstract/router"); }
        node14: Node { @display("p=2000,400;i=abstract/router"); }
        node15: Node { @display("p=2200,500;i=abstract/router"); }
        node16: Node { @display("p=2400,600;i=abstract/router"); }
        node17: Node { @display("p=1000,700;i=abstract/router"); }
        node18: Node { @display("p=1200,800;i=abstract/router"); }
        node19: Node { @display("p=1400,600;i=abstract/router"); }
    connections:
        // Random connections with randomly generated delays between nodes
        node0.gate++ <--> { delay = 45ms; } <--> node1.gate++;
        node0.gate++ <--> { delay = 60ms; } <--> node5.gate++;
        node1.gate++ <--> { delay = 30ms; } <--> node2.gate++;
        node1.gate++ <--> { delay = 50ms; } <--> node6.gate++;
        node2.gate++ <--> { delay = 80ms; } <--> node3.gate++;
        node2.gate++ <--> { delay = 70ms; } <--> node7.gate++;
        node3.gate++ <--> { delay = 90ms; } <--> node4.gate++;
        node3.gate++ <--> { delay = 100ms; } <--> node8.gate++;
        node4.gate++ <--> { delay = 110ms; } <--> node9.gate++;
        node5.gate++ <--> { delay = 120ms; } <--> node6.gate++;
        node6.gate++ <--> { delay = 130ms; } <--> node7.gate++;
        node7.gate++ <--> { delay = 140ms; } <--> node8.gate++;
        node8.gate++ <--> { delay = 150ms; } <--> node9.gate++;
        node9.gate++ <--> { delay = 160ms; } <--> node10.gate++;
        node10.gate++ <--> { delay = 170ms; } <--> node11.gate++;
        node11.gate++ <--> { delay = 180ms; } <--> node12.gate++;
        node12.gate++ <--> { delay = 190ms; } <--> node13.gate++;
        node13.gate++ <--> { delay = 200ms; } <--> node14.gate++;
        node14.gate++ <--> { delay = 210ms; } <--> node15.gate++;
        node15.gate++ <--> { delay = 220ms; } <--> node16.gate++;
        node16.gate++ <--> { delay = 230ms; } <--> node17.gate++;
        node17.gate++ <--> { delay = 240ms; } <--> node18.gate++;
        node18.gate++ <--> { delay = 250ms; } <--> node19.gate++;
}
