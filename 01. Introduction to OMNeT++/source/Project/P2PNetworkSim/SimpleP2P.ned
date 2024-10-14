network SimpleP2P
{
    types:
        channel ChannelType extends ned.DatarateChannel {
            datarate = 1Gbps;
            delay = 10ms;
        }

    submodules:
        nodeA: StandardHost;
        nodeB: StandardHost;

    connections:
        nodeA.ethg++ <--> ChannelType <--> nodeB.ethg++;
}
