#include "InterContactTimeObserver.h"

Define_Module(InterContactTimeObserver);

omnetpp::simsignal_t 
  InterContactTimeObserver::linkLifetime = registerSignal("linkLifetime");
omnetpp::simsignal_t 
  InterContactTimeObserver::interContactTime = registerSignal("interContactTime");

InterContactTimeObserver::InterContactTimeObserver() :
  ict_counter(0), llt_counter(0)
{ }

void InterContactTimeObserver::initialize(int stage) {
  if (stage == 0) {
    PositionObserver::initialize(stage);
    ict_num = par("observations");
    ict_min = par("minICT");
    llt_min = par("minLLT");
    llt.resize(node_number);
    ictt.resize(node_number);
    WATCH(ict_counter);
    WATCH(llt_counter);
  }
}

std::unordered_map<unsigned, omnetpp::simtime_t>
InterContactTimeObserver::computeOneHopNeighborhood(unsigned node_id) {
  std::unordered_map <unsigned, omnetpp::simtime_t> neighborhood;
  unsigned square = computeSquare(node_position[node_id]);
  std::list<unsigned> squareList(std::move(computeNeighboringSquares(square)));
  for (auto& square : squareList) {
    for (auto& neighborId: node_map[square]) {
      if (neighborId != node_id) {
        double distance = sqrt (
          pow(node_position[node_id].x - node_position[neighborId].x, 2) + 
          pow(node_position[node_id].y - node_position[neighborId].y, 2)
        );
        EV_INFO << "Distance between " << node_id << " and " << neighborId << " is " << distance << '\n';
        if (distance < radius) //Nodes are neighbors
          neighborhood[neighborId] = omnetpp::simTime();
      }
    }
  }
  EV_INFO << "Current neighborhood of node: " << node_id << " : ";
  for (auto& entry : neighborhood)
    EV_INFO << entry.first << ' ';
  EV_INFO << '\n';
  return neighborhood;
}

void InterContactTimeObserver::receiveSignal(omnetpp::cComponent* src, omnetpp::simsignal_t id, omnetpp::cObject* value, omnetpp::cObject* details) {
  PositionObserver::receiveSignal(src, id, value, details);
  std::unordered_map<unsigned, omnetpp::simtime_t> n( //current neighborhood
    computeOneHopNeighborhood(node_id)
  );
  std::unordered_map<unsigned, omnetpp::simtime_t> oldN; //old neighbors
  std::unordered_map<unsigned, omnetpp::simtime_t> newN; //new neighbors

  EV_INFO << "Last neighborhood of node " << node_id << " : ";
  for (auto& entry : llt[node_id])
    EV_INFO << entry.first << ' ';
  EV_INFO << '\n';


  //Computes old neighbors
  for (auto& entry : llt[node_id])
    if (n.find(entry.first) == n.end())
      oldN[entry.first] = entry.second;
  //Computes new neighbors
  for (auto& entry : n)
    if (llt[node_id].find(entry.first) == n.end())
      newN[entry.first] = entry.second;

  EV_INFO << "Old neighbors of node " << node_id << " : ";
  for (auto& neighbor : oldN) 
    EV_INFO << neighbor.first << ' ';
  EV_INFO << '\n';

  EV_INFO << "New neighbors of node " << node_id << " : ";
  for (auto& neighbor : newN) 
    EV_INFO << neighbor.first << ' ';
  EV_INFO << '\n';
  
  for (auto& entry: oldN) {
    omnetpp::simtime_t lifetime = omnetpp::simTime() - llt[node_id][entry.first];
    if (lifetime > llt_min) {
      ictt[node_id][entry.first] = omnetpp::simTime();
      ictt[entry.first][node_id] = omnetpp::simTime();
      if (omnetpp::simTime() >= getSimulation()->getWarmupPeriod()) {
        emit(linkLifetime, lifetime);
        llt_counter++;
      }
    }
    llt[node_id].erase(entry.first);
    llt[entry.first].erase(node_id);
    EV_INFO << "Link between node " << node_id << " and node " << entry.first 
      << " is broken at " << omnetpp::simTime() << ", it lasts for: " << lifetime << '\n';
  }
  for (auto& entry: newN) {
    EV_INFO << "A link between node " << node_id << " and " << entry.first << " has been established at time "<< omnetpp::simTime() << '\n';
    if (ictt[node_id].find(entry.first) != ictt[node_id].end()) {
      omnetpp::simtime_t ict = omnetpp::simTime() - ictt[node_id][entry.first];
      if (ict > ict_min) {
        if (omnetpp::simTime() >= getSimulation()->getWarmupPeriod()) {
          emit(interContactTime, ict);
          ict_counter++;
        }
      }
      ictt[node_id].erase(entry.first);
      ictt[entry.first].erase(node_id);
      EV_INFO << "Node " << node_id << " and node " << entry.first 
        << " are neighbors again after: " << ict << '\n';
    }
    //Insertion is a symmetric operation
    llt[node_id][entry.first] = omnetpp::simTime();
    llt[entry.first][node_id] = omnetpp::simTime();
  }
  if (ict_counter > ict_num) {
    EV_INFO << ict_counter << ' ' << ict_num <<'\n';
    endSimulation();
  }
}
