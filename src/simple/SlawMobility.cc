#include "../simple/SlawMobility.h"
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

SlawMobility::SlawMobility() : 
  walkerID(0), nextMoveIsWait(false), slaw(nullptr) 
{ }

void SlawMobility::initialize(int stage) {
  LineSegmentsMobilityBase::initialize(stage);
  if(stage == 0) {
    walkerID = getContainingNode(this)->getIndex();
    classifyFlight = par("classifyFlight").boolValue();
    auto slawModuleName = par("slawModuleName").stringValue();
    slaw = (ISlawTripManager*) this->getSimulation()->
      getSystemModule()->getSubmodule(slawModuleName);
    if(!slaw)
      error("Slaw Mobility: No destination generator found, add module Slaw to the network");
    std::cout << "Mobility state of walker " << walkerID << "\n";
    slaw->setWalkerState(walkerID, C_k, unvisitedWaypointList, lastPosition);
    std::cout << "number of confined areas: " << C_k.size() << "\n\t"
      << "Start: " << lastPosition << "\n\t"
      << "trip size: " << unvisitedWaypointList.size() << '\n'
      << "areas: ";
    for (auto& areaNumber: C_k)
        std::cout << areaNumber << ' ';
    std::cout << '\n';
    int tripSize = unvisitedWaypointList.size();
    emit(trip_size, tripSize);
  }
}


void SlawMobility::setInitialPosition() {
  emit(next_waypoint, &lastPosition);
}

void SlawMobility::setTargetPosition()
{
  if (nextMoveIsWait)
    nextChange = omnetpp::simTime() + slaw->pauseTimeModel->computePausetime();
  else {
    isNewTrip = (unvisitedWaypointList.empty());
    targetPosition = slaw->getNextDestination(
      unvisitedWaypointList, C_k, lastPosition, walkerID
    );
    distance = lastPosition.distance(targetPosition);
    nextChange = omnetpp::simTime() + distance / slaw->speedModel->computeSpeed();
    emitSignals();
  }
  nextMoveIsWait = !nextMoveIsWait;
}

void SlawMobility::move() {
  LineSegmentsMobilityBase::move();
  raiseErrorIfOutside();
}

void SlawMobility::emitSignals() {
  emit(next_waypoint, &targetPosition);
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
