#include "GlobalConnectivityObserver.h"
#include <cstdio>
Define_Module(GlobalConnectivityObserver);

omnetpp::simsignal_t GlobalConnectivityObserver::degree = registerSignal("degree");

GlobalConnectivityObserver::GlobalConnectivityObserver()
  : msg(nullptr)
  , neighborhood_list(nullptr)
{ }

GlobalConnectivityObserver::~GlobalConnectivityObserver() {
  if (neighborhood_list) {
    delete(neighborhood_list);
    EV_INFO << "ConnectivityObserver: Delete neighborhood list\n";
  }
}

void GlobalConnectivityObserver::initialize(int stage) {
  if (stage == 0) {
    PositionObserver::initialize(stage);
    filename = par("filename").stringValue();
    sample_size = par("observations");
    neighborhood_list = new std::vector< std::list<unsigned> >(node_number);
    adjacency_matrix.initialize(node_number);
  }
  else if (stage == 2) {
    msg = new omnetpp::cMessage();
    msg->setSchedulingPriority(255); //the lowest priority
    scheduleAt(omnetpp::simTime(), msg);
  }
}

std::list<unsigned> 
GlobalConnectivityObserver::computeOneHopNeighborhood(unsigned id) {
  std::list<unsigned> neighborhood;
  unsigned square = computeSquare(node_position[id]);
  std::list<unsigned> squareList(std::move(computeNeighboringSquares(square)));
  for (auto& square : squareList) {
    for (auto& neighborId: node_map[square]) {
      if (neighborId != id) {
        double distance = sqrt (
          pow(node_position[id].x - node_position[neighborId].x, 2) + 
          pow(node_position[id].y - node_position[neighborId].y, 2)
        );
        EV_INFO << "Distance between " << id << " and " << neighborId << " is " << distance << '\n';
        //Nodes are neighbors being at the observation area
        EV_INFO << "Node position: " << node_position[id] << " at time: " << omnetpp::simTime() << '\n';
        EV_INFO << "Neighbor position: " << node_position[neighborId] << " at time: " << omnetpp::simTime() << '\n';
        if (distance < radius) 
          neighborhood.push_back(neighborId);
      }
    }
  }
  return neighborhood;
}

void GlobalConnectivityObserver::receiveSignal(
  omnetpp::cComponent* src, 
  omnetpp::simsignal_t id, 
  omnetpp::cObject* value, 
  omnetpp::cObject* details
) {
  PositionObserver::receiveSignal(src, id, value, details);
}

void GlobalConnectivityObserver::computeNewNeighbors(
  unsigned id,
  std::list<unsigned>& current_neighborhood
) {
  for (auto& cn : current_neighborhood) {
    if (
      std::find_if (
        neighborhood_list->at(id).begin(),
        neighborhood_list->at(id).end(),
        [cn] (unsigned x) { return cn == x; }
      ) == neighborhood_list->at(id).end()
    ) {
      neighborhood_list->at(id).push_back(cn);
      EV_INFO<< "\tnode: " << cn << " is new neighbor of node " 
             << id << " at " << omnetpp::simTime() << "\n";
    }
    else
      adjacency_matrix.get(id, cn)++;
  }
}

void GlobalConnectivityObserver::computeOldNeighbors(
  unsigned id,
  std::list<unsigned>& current_neighborhood
) {
  auto n_it =  neighborhood_list->at(id).begin();
  while (n_it != neighborhood_list->at(id).end()) {
    auto n_jt = std::find_if(
                current_neighborhood.begin(),
                current_neighborhood.end(),
                [n_it] (unsigned x) { return x == *n_it; }
              );
    if (n_jt == current_neighborhood.end()) {
      EV_INFO << "Erase node: " << *n_it << '\n';
      neighborhood_list->at(id).erase(n_it++);
    }
    else
      ++n_it;
  }
}

void GlobalConnectivityObserver::handleMessage(omnetpp::cMessage* msg) {
  if (msg->isSelfMessage()) {
    EV_INFO << "Simulation time: " << omnetpp::simTime() <<'\n';
    for (size_t id = 0; id < node_number; id++) {
      std::list<unsigned> current_neighborhood = 
        computeOneHopNeighborhood(id);
      emit(degree, current_neighborhood.size());
      std::list<unsigned> new_neighbor;

      EV_INFO << "Current neighborhood of node " 
              << id << ':';
      for (auto& nid : current_neighborhood)
        EV_INFO << ' ' << nid;
      EV_INFO << "\n";

      computeNewNeighbors(id, current_neighborhood);

      EV_INFO << "neighbor list of node: " << id << ':';
      for (auto& nid : neighborhood_list->at(id))
        EV_INFO << ' ' << nid;
      EV_INFO << '\n';

      computeOldNeighbors(id, current_neighborhood);
    }
    scheduleAt(omnetpp::simTime() + 1.0, msg);

    EV_INFO << "Adjacency matrix:\n";
    EV_INFO << adjacency_matrix;
  }
  else
    error("GlobalConnectivityObserver: This module does not receive messages\n");
}

void GlobalConnectivityObserver::finish() {
  adjacency_matrix.write_ratio(filename, omnetpp::simTime().dbl());
}