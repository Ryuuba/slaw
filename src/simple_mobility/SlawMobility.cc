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
    slawModuleName = par("slawModuleName").stringValue();
    slaw = (SlawEngine*) this->getSimulation()->getSystemModule()->getSubmodule(slawModuleName.c_str());
    if(!slaw)
      error("No destination generator found: add module Slaw to the\
        network");
    std::cout << "Mobility state of walker " << walkerID << "\n";
    slaw->initializeMobilityState(trip, areas, home, walkerID);
    std::cout << "number of confined areas: " << areas.size() << "\n\t"
      << "home: " << home << "\n\t"
      << "trip size: " << trip.size() << '\n'
      << "areas: ";
    for (auto& areaNumber: areas)
        std::cout << areaNumber << ' ';
    std::cout << '\n';
  }
}

void SlawMobility::setInitialPosition() {
  lastPosition = home;
  emit(next_waypoint, &lastPosition);
}

void SlawMobility::setTargetPosition()
{
  bool isNewTrip = false;
  if(nextMoveIsWait)
    nextChange = simTime() + slaw->computePauseTime();
  else{
    if(trip.empty()) {
      isNewTrip = true;
      if (slaw->isSLAW_MATLAB())
      {
        slaw->computeTrip(trip, areas, lastPosition);
        targetPosition = slaw->LATP(lastPosition, trip);
      }
      else
      {
        slaw->computeTrip(trip, areas, home);
        targetPosition = home;
      }
      counter++;
      emit(next_waypoint, &targetPosition);
      emit(trip_counter, counter);
      std::cout << "SLAW Mobility: Trip is finished\n";
    }
    if (!isNewTrip) //Avoids emitting the home waypoint two times
    {
      targetPosition = slaw->LATP(lastPosition, trip);
      emit(next_waypoint, &targetPosition);
    }
    double distance = lastPosition.distance(targetPosition);
    nextChange = simTime() + distance / slaw->getSpeed(distance);
    if (!classifyFlight)
      emit(flight, distance);
    else {
      if (slaw->sameArea(lastPosition, targetPosition))
        emit(intraFlightLength, distance);
      else 
        emit(interFlightLength, distance);
    }
  }
  nextMoveIsWait = !nextMoveIsWait;
}

void SlawMobility::move() {
  LineSegmentsMobilityBase::move();
  raiseErrorIfOutside();
}
