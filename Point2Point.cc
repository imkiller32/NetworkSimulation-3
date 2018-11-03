#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main()
{
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);

	NodeContainer nodes;
	nodes.Create(2);

	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute("DataRate",StringValue("2Mbps"));
	pointToPoint.SetChannelAttribute("Delay",StringValue("1ms"));

	NetDeviceContainer devices = pointToPoint.Install(nodes);

	InternetStackHelper stack;
	stack.Install(nodes);

	Ipv4AddressHelper address;
	address.SetBase("10.1.1.0","255.25.255.0");

	Ipv4InterfaceContainer interfaces = address.Assign(devices);

	UdpEchoServerHelper echoServer(9);
	ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
	serverApps.Start(Seconds(1.0));
	serverApps.Stop(Seconds(8.0));

	UdpEchoClientHelper echoClient(interfaces.GetAddress(1),9);
	ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
	clientApps.Start(Seconds(3.0));
	clientApps.Stop(Seconds(7.0));

	AnimationInterface anim ("P2P.xml");
	
	anim.SetConstantPosition(nodes.Get(0),200,200);
	anim.SetConstantPosition(nodes.Get(1),300,300);

	Simulator::Run();
	Simulator::Destroy();

}
