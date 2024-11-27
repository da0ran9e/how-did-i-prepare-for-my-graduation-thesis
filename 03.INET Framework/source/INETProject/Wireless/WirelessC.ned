import inet.environment.common.PhysicalEnvironment;


network WirelessC extends WirelessB
{
    submodules:
        physicalEnvironment: PhysicalEnvironment {
            @display("p=580,425");
        }
}