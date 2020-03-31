#include "ConnectivityObserver.h"
Define_Module(ConnectivityObserver);

omnetpp::simsignal_t ConnectivityObserver::membership_stat = registerSignal("membership");
omnetpp::simsignal_t ConnectivityObserver::arrival_stat = registerSignal("arrival");
omnetpp::simsignal_t ConnectivityObserver::departure_stat = registerSignal("departure");

ConnectivityObserver::ConnectivityObserver()
  : msg(nullptr)
  , filename(nullptr)
  , llt_min(0.0)
  , sample_size(0)
  , membership_size(0)
  , observation_counter(0)
  , arrival_num(0)
  , departure_num(0)
  { }

ConnectivityObserver::~ConnectivityObserver() {
  if (neighborhood_list) {
    delete(neighborhood_list);
    EV_INFO << "ConnectivityObserver: Delete neighborhood list\n";
  }
}

void ConnectivityObserver::initialize(int stage) {
  if (stage == 0) {
    PositionObserver::initialize(stage);
    llt_min = par("minLLT");
    filename = par("filename").stringValue();
    sample_size = par("observations");
    neighborhood_list = new std::vector< std::list<unsigned> >(node_number);
    adjacency_matrix.initialize(node_number);
  }
  if (stage == 2) {
    msg = new omnetpp::cMessage();
    msg->setSchedulingPriority(255); //the lowest priority
    scheduleAt(omnetpp::simTime(), msg);
    auto map_ptr = this->getSimulation()->getSystemModule()->
                    getSubmodule("tripmanager")->getSubmodule("mapmodule");
    polygon = reinterpret_cast<SelfSimilarWaypointMap*>(map_ptr)->getConvexHull();
    WATCH(membership_size);
    WATCH(observation_counter);
    WATCH(arrival_num);
    WATCH(departure_num);
  }
}

std::list<unsigned> 
ConnectivityObserver::computeOneHopNeighborhood(unsigned id) {
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
        if ((distance < radius) && isInObservationArea(node_position[neighborId])) 
          neighborhood.push_back(neighborId);
      }
    }
  }
  return neighborhood;
}

void ConnectivityObserver::receiveSignal(
  omnetpp::cComponent* src, 
  omnetpp::simsignal_t id, 
  omnetpp::cObject* value, 
  omnetpp::cObject* details
) {
  PositionObserver::receiveSignal(src, id, value, details);
  auto state = dynamic_cast<inet::MovingMobilityBase*>(value);
  inet::Coord current_position(state->getCurrentPosition());
  auto it = membership.find(node_id);
  if (isInObservationArea(current_position)) {
    if (it == membership.end()) {
      arrival_num++;
      membership.insert(node_id);
    }
  }
  else {
    if (it != membership.end()) {
      membership.erase(it);
      departure_num++;
    }
  } 
}

void ConnectivityObserver::finish() {
  std::ofstream ofs(filename);
  if (ofs.is_open()) {
    for (auto& row : *(adjacency_matrix.get())) {
      for (auto& time : row) {
        if (time > llt_min)
          ofs << time << ' ';
        else
          ofs << 0 << ' ';
      }
      ofs << std::endl;
    }
    ofs.close();
  }
  else 
    error("ConnectivityObserver: %s file couldn't be opened\n", filename);
}

bool ConnectivityObserver::isInObservationArea(inet::Coord& position) {
  auto predicate = CGAL::bounded_side_2(
    polygon->begin(), polygon->end(), point_2(position.x, position.y), K()
  );
  return predicate != CGAL::ON_UNBOUNDED_SIDE;
}

void ConnectivityObserver::computeNewNeighbors(
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
      EV_INFO<< "\tnode: " << cn << " is new neighbor of node " << id << " at " << omnetpp::simTime() << "\n";
    }
    else 
      //This increment only works if nodes emit a signal each second
      adjacency_matrix.get(id, cn)++;
  }
}

void ConnectivityObserver::computeOldNeighbors(
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
      neighborhood_list->at(node_id).erase(n_it++);
    }
    else 
      ++n_it;
  }
}

void ConnectivityObserver::handleMessage(omnetpp::cMessage* msg) {
  if (msg->isSelfMessage()) {
    EV_INFO << "Simulation time: " << omnetpp::simTime() <<'\n';
    EV_INFO << "Number of arrivals: " << arrival_num << '\n';
    EV_INFO << "Number of departures: " << departure_num << '\n';
    EV_INFO << "Membership: ";
    for (auto& member : membership)
      EV_INFO << member << ' ';
    EV_INFO << '\n';
    for (auto& member : membership) {
      std::list<unsigned> current_neighborhood = 
        computeOneHopNeighborhood(member);
      std::list<unsigned> new_neighbor;

      EV_INFO << "Current neighborhood of node " 
                << member << "\n";
      for (auto& nid : current_neighborhood)
        EV_INFO << "\tid: " << nid << "\n";

      computeNewNeighbors(member, current_neighborhood);

      EV_INFO << "neighbor list of node: " << member << '\n';
      for (auto& nid : neighborhood_list->at(member))
        EV_INFO << "neighbor: " << nid << '\n';

      computeOldNeighbors(member, current_neighborhood);
    }
    scheduleAt(omnetpp::simTime()+1.0, msg);
    membership_size = membership.size();
    emit(membership_stat, membership.size());
    emit(departure_stat, departure_num);
    emit(arrival_stat, arrival_num);
    departure_num = 0;
    arrival_num = 0;
  }
  else
    error("Connectivity Observer: This module does not receive messages\n");
}