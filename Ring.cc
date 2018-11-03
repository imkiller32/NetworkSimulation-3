#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

#include <bits/stdc++.h>

using namespace std;
using namespace ns3;

int main()
{
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);

	NodeContainer nodes;
	nodes.Create(5);

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate",StringValue("10Mbps"));
	p2p.SetChannelAttribute("Delay",StringValue("2ms"));

	InternetStackHelper stack;
	stack.Install(nodes);
	
	vector<NetDeviceContainer> devices;
	vector<Ipv4InterfaceContainer> interfaces;
	vector<ApplicationContainer> serverApps,clientApps;

	Ipv4AddressHelper address;

	for(int i=0;i<5;i++)
	{
		string baseip = "10.0.";
		baseip.append(1,'0'+i);
		baseip+=".0";
		
		int j=(i+1)%5;

		address.SetBase(baseip.c_str(),"255.255.255.0");
		devices.push_back(p2p.Install(nodes.Get(i),nodes.Get(j)));
		interfaces.push_back(address.Assign(devices.back()));

		UdpEchoServerHelper echoServer(9+i);
		serverApps.push_back(echoServer.Install(nodes.Get(i)));
		serverApps.back().Start(Seconds(1.0));
		serverApps.back().Stop(Seconds(10.0));

		UdpEchoClientHelper echoClient(interfaces.back().GetAddress(0),9+i);
		clientApps.push_back(echoClient.Install(nodes.Get(j)));
		clientApps.back().Start(Seconds(2.0));
		clientApps.back().Stop(Seconds(9.0));
	}

	AnimationInterface anim("Ring.xml");

	anim.SetConstantPosition(nodes.Get(0),100,100);
	anim.SetConstantPosition(nodes.Get(1),200,100);
	anim.SetConstantPosition(nodes.Get(2),300,100);
	anim.SetConstantPosition(nodes.Get(3),400,100);
	anim.SetConstantPosition(nodes.Get(4),250,50);

	Simulator::Run();
	Simulator::Destroy();
}
