#include "../mobility/SlawMobility.h"
#include <cstring>

Define_Module(SlawMobility);

omnetpp::simsignal_t
  SlawMobility::flight = registerSignal("flight");
omnetpp::simsignal_t
  SlawMobility::intraFlightLength = registerSignal("intraFlightLength");
omnetpp::simsignal_t
  SlawMobility::interFlightLength = registerSignal("interFlightLength");
omnetpp::simsignal_t
  SlawMobility::remote_random_area = registerSignal("remoteRandomArea");
omnetpp::simsignal_t
  SlawMobility::trip_size = registerSignal("tripSize");
omnetpp::simsignal_t
  SlawMobility::next_waypoint = registerSignal("nextWaypoint");

SlawMobility::SlawMobility()
  : walkerID(0)
  , nextMoveIsWait(false)
  , slaw(nullptr) 
{ }

void SlawMobility::initialize(int stage) {
  
  if (stage == inet::INITSTAGE_LOCAL) {
    auto walker_model = par("walkerModuleName").stringValue();
    slaw = (IWalkerModel*) this->getSimulation()->
      getSystemModule()->getSubmodule("tripmanager")->
      getSubmodule(walker_model);
    classifyFlight = par("classifyFlight").boolValue();
    std::cout << "SlawMobility: stage " << stage << " OK\n";
  }
  else if (stage == inet::INITSTAGE_SINGLE_MOBILITY) {
    if(!slaw)
      error("Slaw Mobility: No destination generator found, add module Slaw to the network");
    walkerID = getContainingNode(this)->getIndex();
    EV_INFO << "Mobility state of walker " << walkerID << "\n";
    EV_INFO << "stage: " << stage << '\n';
    std::cout << "Este es el paso de la muerte\n";
    slaw->setWalkerState(walkerID, C_k, unvisitedWaypointList, lastPosition);
    std::cout << "Pasamos el paso de la muerte\n";
    EV_INFO << "number of confined areas: " << C_k.size() << "\n\t"
      << "Start: " << lastPosition << "\n\t"
      << "trip size: " << unvisitedWaypointList.size() << '\n'
      << "areas: ";
    for (auto& areaNumber: C_k)
        EV_INFO << areaNumber << ' ';
    EV_INFO << "\n";
    int tripSize = unvisitedWaypointList.size();
    emit(trip_size, tripSize);
    WATCH(nextChange);
  }
  LineSegmentsMobilityBase::initialize(stage);//FIX this WTF?
}


void SlawMobility::setInitialPosition() {
  nwp.next_waypoint = lastPosition;
  emit(next_waypoint, &nwp);
}

void SlawMobility::setTargetPosition()
{
  if (nextMoveIsWait) {
    nextChange = omnetpp::simTime() + slaw->getPauseTime();
  }
  else {
    isNewTrip = (unvisitedWaypointList.empty());
    targetPosition = slaw->getNextDestination(
      unvisitedWaypointList, C_k, lastPosition, walkerID
    );
    distance = lastPosition.distance(targetPosition);
    nextChange = omnetpp::simTime() + distance / slaw->getSpeed();
    emitSignals();
  }
  nextMoveIsWait = !nextMoveIsWait;
}

void SlawMobility::move() {
  LineSegmentsMobilityBase::move();
  raiseErrorIfOutside();
}

void SlawMobility::emitSignals() {
  nwp.next_waypoint = targetPosition;
  emit(next_waypoint, &nwp);
  if (isNewTrip) {
    int tripSize = unvisitedWaypointList.size(); 
    if (std::strcmp(slaw->getIndividualWalkerModelName(), "SlawMatlab"))
      tripSize++; //getNextDestination draws one wpt when computing a new trip
    emit(trip_size, tripSize);
  }
  if (!classifyFlight)
    emit(flight, distance);
  else {
    if (slaw->map->isSameArea(lastPosition, targetPosition))
      emit(intraFlightLength, distance);
    else {
      emit(interFlightLength, distance);
      int areaId = slaw->map->getAreaID(targetPosition);
      if (C_k.end() == std::find(C_k.begin(), C_k.end(), areaId))
        emit(remote_random_area, true); //next area is not in C_k
      else
        emit(remote_random_area, false);
    }
  }
}
