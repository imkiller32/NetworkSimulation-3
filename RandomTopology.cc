#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("client_server");


int main (int argc, char *argv[])
{
  double lat = 2.0;
  uint64_t rate = 5000000; // Data rate in bps
  double interval = 0.05;


  NS_LOG_INFO ("Create nodes.");
  NodeContainer n;
  n.Create (5);

  NS_LOG_INFO ("Create channels.");

  PointToPointHelper p2p;
  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (lat)));
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (rate)));
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (1400));

  NetDeviceContainer dev = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev2 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev3 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev4 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev5 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev6 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev7 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));
  NetDeviceContainer dev8 = p2p.Install (n.Get(rand()%5), n.Get(rand()%5));



  InternetStackHelper internet;
  internet.Install (n);
  Ipv4AddressHelper ipv4;
NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (dev);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i2 = ipv4.Assign (dev2);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i3 = ipv4.Assign (dev3);

  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i4 = ipv4.Assign (dev4);

  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i5 = ipv4.Assign (dev5);

  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i6 = ipv4.Assign (dev6);

  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i7 = ipv4.Assign (dev7);

   ipv4.SetBase ("10.1.8.0", "255.255.255.0");
   Ipv4InterfaceContainer i8 = ipv4.Assign (dev8);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                   "X", StringValue ("10.0"),
                                   "Y", StringValue ("10.0"),
                                   "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=30]"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

  mobility.Install (n);
  NS_LOG_INFO ("Create Applications.");

  uint16_t port1 = 8000; 
  uint16_t port2 = 8001;
  uint16_t port3 = 8002;
  UdpServerHelper server1 (port1);
  UdpServerHelper server2 (port2);
  UdpServerHelper server3 (port3);
  ns3::

  ApplicationContainer apps;

  apps = server1.Install (n.Get (rand()%5));
  apps = server2.Install (n.Get (rand()%5));
  apps = server3.Install (n.Get (rand()%5));

  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  uint32_t MaxPacketSize = 1024;
  Time interPacketInterval = Seconds (interval);
  uint32_t maxPacketCount = 320;
  UdpClientHelper client1 (i2.GetAddress (1), port1);
  UdpClientHelper client2 (i2.GetAddress (1), port2);
  UdpClientHelper client3 (i4.GetAddress (1), port3);
 
  
  apps = client1.Install (n.Get (rand()%5));
  apps = client2.Install (n.Get (rand()%5));
  apps = client3.Install (n.Get (rand()%5));

  apps.Start (Seconds (0.01));
  apps.Stop (Seconds (10.0));

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds(11.0));

  std::cout<<"Create Xml File\n";
  AnimationInterface anim ("threenode.xml");
  anim.EnablePacketMetadata(true);
  
  anim.UpdateNodeColor(n.Get(rand()%5),222,225,111);
  anim.SetConstantPosition(n.Get(0),20,20);
  anim.SetConstantPosition(n.Get(1),15,20);
  anim.SetConstantPosition(n.Get(2),10,20);
  anim.SetConstantPosition(n.Get(3),15,10);
  anim.SetConstantPosition(n.Get(4),15,30);

 anim.EnablePacketMetadata ();

  Simulator::Run ();
  Simulator::Destroy ();
  std::cout<<"Done Simulation\n";
  return 0;
}
