#include "../simple_mobility/SlawMobility.h"

Define_Module(SlawMobility);

omnetpp::simsignal_t
  SlawMobility::intraFlightLength = registerSignal("intraFlightLength");
omnetpp::simsignal_t
  SlawMobility::flight = registerSignal("flight");
omnetpp::simsignal_t
  SlawMobility::interFlightLength = registerSignal("interFlightLength");
omnetpp::simsignal_t
  SlawMobility::trip_counter = registerSignal("trip_counter");
omnetpp::simsignal_t
  SlawMobility::next_waypoint = registerSignal("next_waypoint");

SlawMobility::SlawMobility() : 
  counter(0), walkerID(0), nextMoveIsWait(false), slaw(nullptr) { }

void SlawMobility::initialize(int stage) {
  LineSegmentsMobilityBase::initialize(stage);
  if(stage == 0) {
    walkerID = getContainingNode(this)->getIndex();
    classifyFlight = par("classifyFlight").boolValue();
    auto slawModuleName = par("slawModuleName").stringValue();
    slaw = (SlawTripManager*) this->getSimulation()->
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
  }
}

void SlawMobility::setInitialPosition() {
  emit(next_waypoint, &lastPosition);
}

void SlawMobility::setTargetPosition()
{
  if (nextMoveIsWait)
    nextChange = simTime() + slaw->pauseTimeModel->computePausetime();
  else {
    isNewTrip = (unvisitedWaypointList.empty());
    targetPosition = slaw->getNextDestination(
      unvisitedWaypointList, C_k, lastPosition, walkerID
    );
    distance = lastPosition.distance(targetPosition);
    nextChange = simTime() + distance / slaw->speedModel->computeSpeed();
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
    counter++;
    emit(trip_counter, counter);
  }
  if (!classifyFlight)
    emit(flight, distance);
  else {
    if (slaw->map->isSameArea(lastPosition, targetPosition))
      emit(intraFlightLength, distance);
    else 
      emit(interFlightLength, distance);
  }
}
