#include "ConnectivityObserver.h"

Define_Module(ConnectivityObserver)

omnetpp::simsignal_t 
  ConnectivityObserver::linkLifetime = registerSignal("linkLifetime");
omnetpp::simsignal_t 
  ConnectivityObserver::interContactTime = registerSignal("interContactTime");

ConnectivityObserver::ConnectivityObserver() 
  : radius(0.0), ict_num(0), llt_num(0), ict_counter(0), llt_counter(0) {}

void ConnectivityObserver::initialize() {
  PositionObserver::initialize();
  radius = par("radius").doubleValue();
  ict_num = par("numOfICTSamples");
  llt_num = par("numOfLLTSamples");
  ict_min = par("minICT");
  llt_min = par("minLLT");
  llt.resize(numOfNodes);
  ictt.resize(numOfNodes);
  WATCH(ict_counter);
  WATCH(llt_counter);
}

std::unordered_map<unsigned, omnetpp::simtime_t>
ConnectivityObserver::computeOneHopNeighborhood(unsigned nodeId){
  std::unordered_map <unsigned, omnetpp::simtime_t> neighborhood;
  QuadrantCoordinate crd(nodePosition[nodeId]);//crd -> coordinate
  std::list<unsigned> qList(computeNeighboringQuadrants(crd.q, crd.subq));
  for (auto& quadrant : qList){
    for (auto& neighborId: nodeMap[quadrant]) {
      if (neighborId != nodeId) {
        double distance = sqrt (
          pow(nodePosition[nodeId].x - nodePosition[neighborId].x, 2) + 
          pow(nodePosition[nodeId].y - nodePosition[neighborId].y, 2)
        );
        EV_INFO << "Distance between " << nodeId << " and " << neighborId << " is " << distance << '\n';
        if (distance < radius) //Nodes are neighbors
          neighborhood[neighborId] = omnetpp::simTime();
      }
    }
  }
  return neighborhood;
}

void ConnectivityObserver::receiveSignal(omnetpp::cComponent* src, omnetpp::simsignal_t id, omnetpp::cObject* value, omnetpp::cObject* details) {
  PositionObserver::receiveSignal(src, id, value, details);
  std::unordered_map<unsigned, omnetpp::simtime_t> n( //current neighborhood
    computeOneHopNeighborhood(nodeId)
  );
  std::unordered_map<unsigned, omnetpp::simtime_t> oldN; //old neighbors
  std::unordered_map<unsigned, omnetpp::simtime_t> newN; //new neighbors
  EV_INFO << "Current neighborhood of node " << nodeId << " : ";
  for (auto& entry : n)
    EV_INFO << entry.first << ' ';
  EV_INFO << '\n';

  EV_INFO << "Last neighborhood of node " << nodeId << " : ";
  for (auto& entry : llt[nodeId])
    EV_INFO << entry.first << ' ';
  EV_INFO << '\n';


  //Computes old neighbors
  for (auto& entry : llt[nodeId])
    if (n.find(entry.first) == n.end())
      oldN[entry.first] = entry.second;
  //Computes new neighbors
  for (auto& entry : n)
    if (llt[nodeId].find(entry.first) == n.end())
      newN[entry.first] = entry.second;

  EV_INFO << "Old neighbors of node " << nodeId << " : ";
  for (auto& neighbor : oldN) 
    EV_INFO << neighbor.first << ' ';
  EV_INFO << '\n';

  EV_INFO << "New neighbors of node " << nodeId << " : ";
  for (auto& neighbor : newN) 
    EV_INFO << neighbor.first << ' ';
  EV_INFO << '\n';
  
  for (auto& entry: oldN) {
    omnetpp::simtime_t lifetime = omnetpp::simTime() - llt[nodeId][entry.first];
    if (lifetime > llt_min) {
      ictt[nodeId][entry.first] = omnetpp::simTime();
      ictt[entry.first][nodeId] = omnetpp::simTime();
      emit(linkLifetime, lifetime);
      llt_counter++;
    }
    llt[nodeId].erase(entry.first);
    llt[entry.first].erase(nodeId);
    EV_INFO << "Link between node " << nodeId << " and node " << entry.first 
      << " is broken at " << omnetpp::simTime() << ", it lasts for: " << lifetime << '\n';
  }
  for (auto& entry: newN) {
    EV_INFO << "A link between node " << nodeId << " and " << entry.first << " has been established at time "<< omnetpp::simTime() << '\n';
    if (ictt[nodeId].find(entry.first) != ictt[nodeId].end()) {
      omnetpp::simtime_t ict = omnetpp::simTime() - ictt[nodeId][entry.first];
      if (ict > ict_min) {
        emit(interContactTime, ict);
        ict_counter++;
      }
      ictt[nodeId].erase(entry.first);
      ictt[entry.first].erase(nodeId);
      EV_INFO << "Node " << nodeId << " and node " << entry.first 
        << " are neighbors again after: " << ict << '\n';
    }
    //Insertion is a symmetric operation
    llt[nodeId][entry.first] = omnetpp::simTime();
    llt[entry.first][nodeId] = omnetpp::simTime();
  }
  if (llt_counter > llt_num && ict_counter > ict_num)
    endSimulation();
}