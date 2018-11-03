#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

#include<bits/stdc++.h>

using namespace ns3;
using namespace std;

int main()
{
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);

	NodeContainer nodes,hub;
	hub.Create(1);
	nodes.Create(4);

	InternetStackHelper stack;
        stack.Install(hub);
        stack.Install(nodes);

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate",StringValue("100Mbps"));
	p2p.SetChannelAttribute("Delay",StringValue("2ms"));

	vector<NetDeviceContainer> devices;
	vector<Ipv4InterfaceContainer> interfaces;
	vector<ApplicationContainer> serverApps,clientApps;

	Ipv4AddressHelper address;

	for(int i=0;i<4;i++)
	{
		string baseip = "10.0.";
		baseip.append(1,'0'+i);
		baseip+=".0";
		address.SetBase(baseip.c_str(),"255.255.255.0");
		devices.push_back(p2p.Install(hub.Get(0),nodes.Get(i)));
		interfaces.push_back(address.Assign(devices.back()));

		UdpEchoServerHelper echoServer(9+i);
		serverApps.push_back(echoServer.Install(hub.Get(0)));
		serverApps.back().Start(Seconds(0.0));
		serverApps.back().Stop(Seconds(10.0));

		UdpEchoClientHelper echoClient(interfaces.back().GetAddress(0),9+i);
		clientApps.push_back(echoClient.Install(nodes.Get(i)));
		clientApps.back().Start(Seconds(2.0+i));
		clientApps.back().Stop(Seconds(10.0));
	}
	AnimationInterface anim("StarTopology.xml");

	anim.SetConstantPosition(hub.Get(0),300,300);
	anim.SetConstantPosition(nodes.Get(0),200,300);
	anim.SetConstantPosition(nodes.Get(1),400,300);
	anim.SetConstantPosition(nodes.Get(2),300,400);
	anim.SetConstantPosition(nodes.Get(3),300,200);

	Simulator::Run();
	Simulator::Destroy();
}
