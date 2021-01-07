#include "ChurnObserver.h"
Define_Module(ChurnObserver);

omnetpp::simsignal_t ChurnObserver::membership_stat = registerSignal("membership");
omnetpp::simsignal_t ChurnObserver::arrival_stat = registerSignal("arrival");
omnetpp::simsignal_t ChurnObserver::departure_stat = registerSignal("departure");
omnetpp::simsignal_t 
  ChurnObserver::position = registerSignal("mobilityStateChanged");

ChurnObserver::ChurnObserver()
  : timer(nullptr)
  , membership_size(0)
  , arrival_num(0)
  , departure_num(0) 
{ 
  getSimulation()->getSystemModule()->subscribe(position, this);
}

ChurnObserver::~ChurnObserver() {
  cancelAndDelete(timer);
  getSimulation()->getSystemModule()->unsubscribe(position, this);
}

void ChurnObserver::initialize(int stage) {
  SelfSimilarWaypointMap* map_ptr;
  if (stage == inet::INITSTAGE_LOCAL) {
    sample_size = par("observations");
    map_ptr = (SelfSimilarWaypointMap*) this->getSimulation()->getSystemModule()->getSubmodule("tripmanager")->getSubmodule(par("mapModule").stringValue());
    polygon = map_ptr->getConvexHull();
  }
  else if (stage == inet::INITSTAGE_SINGLE_MOBILITY) {
    timer = new omnetpp::cMessage();
    timer->setSchedulingPriority(255); //the lowest priority
    scheduleAt(omnetpp::simTime(), timer);
    WATCH(membership_size);
    WATCH(counter);
    WATCH(arrival_num);
    WATCH(departure_num);
  }
}

void ChurnObserver::receiveSignal(
  omnetpp::cComponent* src, 
  omnetpp::simsignal_t id, 
  omnetpp::cObject* value, 
  omnetpp::cObject* details
) {
  auto node_id = dynamic_cast<omnetpp::cModule*>(src)->getParentModule()->getIndex();
  auto state = dynamic_cast<inet::MovingMobilityBase*>(value);
  inet::Coord current_position(state->getCurrentPosition());
  auto it = membership.find(node_id);
  auto temp = isInObservationArea(current_position);
  if (temp) {
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

bool ChurnObserver::isInObservationArea(inet::Coord& position) {
  auto predicate = CGAL::bounded_side_2(
    polygon->begin(), polygon->end(), point_2(position.x, position.y), K()
  );
  return predicate != CGAL::ON_UNBOUNDED_SIDE;
}


void ChurnObserver::handleMessage(omnetpp::cMessage* timer) {
  if (timer->isSelfMessage()) {
    EV_INFO << "Simulation time: " << omnetpp::simTime() <<'\n';
    EV_INFO << "Number of arrivals: " << arrival_num << '\n';
    EV_INFO << "Number of departures: " << departure_num << '\n';
    EV_INFO << "Membership: ";
    for (auto& member : membership)
      EV_INFO << member << ' ';
    EV_INFO << '\n';
    scheduleAt(omnetpp::simTime() + 1.0, timer);
    membership_size = membership.size();
    emit(membership_stat, membership.size());
    emit(departure_stat, departure_num);
    emit(arrival_stat, arrival_num);
    departure_num = 0;
    arrival_num = 0;
    counter++;
    if (counter == sample_size)
      endSimulation();
  }
  else
    error("ChurnObserver: This module does not receive messages\n");
}