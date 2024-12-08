import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.INetworkNode;
import inet.applications.contract.IApp;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.common.scenario.ScenarioManager;

network WirelessA
{
    parameters:
        @display("bgb=650,500;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostA.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=580,125");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=580,275");
        }


        hostA: <default("WirelessHost")> like INetworkNode {
            @display("p=385,215");
        }
        host01: <default("WirelessHost")> like INetworkNode {
            @display("p=85,215");
        }

}
network WirelessB
{
    parameters:
        @display("bgb=650,500;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostA.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=580,125");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=580,275");
        }
        scenarioManager: ScenarioManager {
            parameters:
                @display("p=580,347.69232");
        }
        hostA: <default("WirelessHost")> like INetworkNode {
            @display("p=385,215");
        }
        host01: <default("WirelessHost")> like INetworkNode {
            @display("p=85,215");
        }

}
network WirelessC
{
    parameters:
        @display("bgb=1648.0801,1103.76;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=mobileHost.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=1579.536,603.792");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=1579.536,666.288");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=1580,725");
        }
        scenarioManager: ScenarioManager {
            parameters:
                @display("p=1578.582,814.752");
        }
        mobileHost: <default("WirelessHost")> like INetworkNode {
            @display("p=1218.672,626.3712");
        }
        host00: <default("WirelessHost")> like INetworkNode {
            @display("p=85,115");
        }
        host01: <default("WirelessHost")> like INetworkNode {
            @display("p=185,115");
        }
        host02: <default("WirelessHost")> like INetworkNode {
            @display("p=285,115");
        }
        host03: <default("WirelessHost")> like INetworkNode {
            @display("p=385,115");
        }
        host04: <default("WirelessHost")> like INetworkNode {
            @display("p=485,115");
        }
        host05: <default("WirelessHost")> like INetworkNode {
            @display("p=585,115");
        }
        host06: <default("WirelessHost")> like INetworkNode {
            @display("p=685,115");
        }
        host07: <default("WirelessHost")> like INetworkNode {
            @display("p=785,115");
        }
        host08: <default("WirelessHost")> like INetworkNode {
            @display("p=885,115");
        }
        host09: <default("WirelessHost")> like INetworkNode {
            @display("p=985,115");
        }
        host10: <default("WirelessHost")> like INetworkNode {
            @display("p=85,215");
        }
        host11: <default("WirelessHost")> like INetworkNode {
            @display("p=185,215");
        }
        host12: <default("WirelessHost")> like INetworkNode {
            @display("p=285,215");
        }
        host13: <default("WirelessHost")> like INetworkNode {
            @display("p=385,215");
        }
        host14: <default("WirelessHost")> like INetworkNode {
            @display("p=485,215");
        }
        host15: <default("WirelessHost")> like INetworkNode {
            @display("p=585,215");
        }
        host16: <default("WirelessHost")> like INetworkNode {
            @display("p=685,215");
        }
        host17: <default("WirelessHost")> like INetworkNode {
            @display("p=785,215");
        }
        host18: <default("WirelessHost")> like INetworkNode {
            @display("p=885,215");
        }
        host19: <default("WirelessHost")> like INetworkNode {
            @display("p=985,215");
        }
        host20: <default("WirelessHost")> like INetworkNode {
            @display("p=85,315");
        }
        host21: <default("WirelessHost")> like INetworkNode {
            @display("p=185,315");
        }
        host22: <default("WirelessHost")> like INetworkNode {
            @display("p=285,315");
        }
        host23: <default("WirelessHost")> like INetworkNode {
            @display("p=385,315");
        }
        host24: <default("WirelessHost")> like INetworkNode {
            @display("p=485,315");
        }
        host25: <default("WirelessHost")> like INetworkNode {
            @display("p=585,315");
        }
        host26: <default("WirelessHost")> like INetworkNode {
            @display("p=685,315");
        }
        host27: <default("WirelessHost")> like INetworkNode {
            @display("p=785,315");
        }
        host28: <default("WirelessHost")> like INetworkNode {
            @display("p=885,315");
        }
        host29: <default("WirelessHost")> like INetworkNode {
            @display("p=985,315");
        }
        host30: <default("WirelessHost")> like INetworkNode {
            @display("p=85,415");
        }
        host31: <default("WirelessHost")> like INetworkNode {
            @display("p=185,415");
        }
        host32: <default("WirelessHost")> like INetworkNode {
            @display("p=285,415");
        }
        host33: <default("WirelessHost")> like INetworkNode {
            @display("p=385,415");
        }
        host34: <default("WirelessHost")> like INetworkNode {
            @display("p=485,415");
        }
        host35: <default("WirelessHost")> like INetworkNode {
            @display("p=585,415");
        }
        host36: <default("WirelessHost")> like INetworkNode {
            @display("p=685,415");
        }
        host37: <default("WirelessHost")> like INetworkNode {
            @display("p=785,415");
        }
        host38: <default("WirelessHost")> like INetworkNode {
            @display("p=885,415");
        }
        host39: <default("WirelessHost")> like INetworkNode {
            @display("p=985,415");
        }
        host40: <default("WirelessHost")> like INetworkNode {
            @display("p=85,515");
        }
        host41: <default("WirelessHost")> like INetworkNode {
            @display("p=185,515");
        }
        host42: <default("WirelessHost")> like INetworkNode {
            @display("p=285,515");
        }
        host43: <default("WirelessHost")> like INetworkNode {
            @display("p=385,515");
        }
        host44: <default("WirelessHost")> like INetworkNode {
            @display("p=485,515");
        }
        host45: <default("WirelessHost")> like INetworkNode {
            @display("p=585,515");
        }
        host46: <default("WirelessHost")> like INetworkNode {
            @display("p=685,515");
        }
        host47: <default("WirelessHost")> like INetworkNode {
            @display("p=785,515");
        }
        host48: <default("WirelessHost")> like INetworkNode {
            @display("p=885,515");
        }
        host49: <default("WirelessHost")> like INetworkNode {
            @display("p=985,515");
        }
        host50: <default("WirelessHost")> like INetworkNode {
            @display("p=85,615");
        }
        host51: <default("WirelessHost")> like INetworkNode {
            @display("p=185,615");
        }
        host52: <default("WirelessHost")> like INetworkNode {
            @display("p=285,615");
        }
        host53: <default("WirelessHost")> like INetworkNode {
            @display("p=385,615");
        }
        host54: <default("WirelessHost")> like INetworkNode {
            @display("p=485,615");
        }
        host55: <default("WirelessHost")> like INetworkNode {
            @display("p=585,615");
        }
        host56: <default("WirelessHost")> like INetworkNode {
            @display("p=685,615");
        }
        host57: <default("WirelessHost")> like INetworkNode {
            @display("p=785,615");
        }
        host58: <default("WirelessHost")> like INetworkNode {
            @display("p=885,615");
        }
        host59: <default("WirelessHost")> like INetworkNode {
            @display("p=985,615");
        }
        host60: <default("WirelessHost")> like INetworkNode {
            @display("p=85,715");
        }
        host61: <default("WirelessHost")> like INetworkNode {
            @display("p=185,715");
        }
        host62: <default("WirelessHost")> like INetworkNode {
            @display("p=285,715");
        }
        host63: <default("WirelessHost")> like INetworkNode {
            @display("p=385,715");
        }
        host64: <default("WirelessHost")> like INetworkNode {
            @display("p=485,715");
        }
        host65: <default("WirelessHost")> like INetworkNode {
            @display("p=585,715");
        }
        host66: <default("WirelessHost")> like INetworkNode {
            @display("p=685,715");
        }
        host67: <default("WirelessHost")> like INetworkNode {
            @display("p=785,715");
        }
        host68: <default("WirelessHost")> like INetworkNode {
            @display("p=885,715");
        }
        host69: <default("WirelessHost")> like INetworkNode {
            @display("p=985,715");
        }
        host70: <default("WirelessHost")> like INetworkNode {
            @display("p=85,815");
        }
        host71: <default("WirelessHost")> like INetworkNode {
            @display("p=185,815");
        }
        host72: <default("WirelessHost")> like INetworkNode {
            @display("p=285,815");
        }
        host73: <default("WirelessHost")> like INetworkNode {
            @display("p=385,815");
        }
        host74: <default("WirelessHost")> like INetworkNode {
            @display("p=485,815");
        }
        host75: <default("WirelessHost")> like INetworkNode {
            @display("p=585,815");
        }
        host76: <default("WirelessHost")> like INetworkNode {
            @display("p=685,815");
        }
        host77: <default("WirelessHost")> like INetworkNode {
            @display("p=785,815");
        }
        host78: <default("WirelessHost")> like INetworkNode {
            @display("p=885,815");
        }
        host79: <default("WirelessHost")> like INetworkNode {
            @display("p=985,815");
        }
        host80: <default("WirelessHost")> like INetworkNode {
            @display("p=85,915");
        }
        host81: <default("WirelessHost")> like INetworkNode {
            @display("p=185,915");
        }
        host82: <default("WirelessHost")> like INetworkNode {
            @display("p=285,915");
        }
        host83: <default("WirelessHost")> like INetworkNode {
            @display("p=385,915");
        }
        host84: <default("WirelessHost")> like INetworkNode {
            @display("p=485,915");
        }
        host85: <default("WirelessHost")> like INetworkNode {
            @display("p=585,915");
        }
        host86: <default("WirelessHost")> like INetworkNode {
            @display("p=685,915");
        }
        host87: <default("WirelessHost")> like INetworkNode {
            @display("p=785,915");
        }
        host88: <default("WirelessHost")> like INetworkNode {
            @display("p=885,915");
        }
        host89: <default("WirelessHost")> like INetworkNode {
            @display("p=985,915");
        }
        host90: <default("WirelessHost")> like INetworkNode {
            @display("p=85,1015");
        }
        host91: <default("WirelessHost")> like INetworkNode {
            @display("p=185,1015");
        }
        host92: <default("WirelessHost")> like INetworkNode {
            @display("p=285,1015");
        }
        host93: <default("WirelessHost")> like INetworkNode {
            @display("p=385,1015");
        }
        host94: <default("WirelessHost")> like INetworkNode {
            @display("p=485,1015");
        }
        host95: <default("WirelessHost")> like INetworkNode {
            @display("p=585,1015");
        }
        host96: <default("WirelessHost")> like INetworkNode {
            @display("p=685,1015");
        }
        host97: <default("WirelessHost")> like INetworkNode {
            @display("p=785,1015");
        }
        host98: <default("WirelessHost")> like INetworkNode {
            @display("p=885,1015");
        }
        host99: <default("WirelessHost")> like INetworkNode {
            @display("p=985,1015");
        }
}
