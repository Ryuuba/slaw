#include "../simple_mobility/SlawMobility.h"

Define_Module(SlawMobility);

omnetpp::simsignal_t
  SlawMobility::intraFlightLength = registerSignal("intraFlightLength");
omnetpp::simsignal_t
  SlawMobility::flight = registerSignal("flight");
omnetpp::simsignal_t
  SlawMobility::interFlightLength = registerSignal("interFlightLength");

SlawMobility::SlawMobility()
{
  nextMoveIsWait = false;
  slaw = nullptr;
}

void SlawMobility::initialize(int stage) {
  LineSegmentsMobilityBase::initialize(stage);
  if(stage == 0) {
    classifyFlight = par("classifyFlight").boolValue();
    slawModuleName = par("slawModuleName").stringValue();
    slaw = (SlawEngine*) this->getSimulation()->getSystemModule()->getSubmodule(slawModuleName.c_str());
    if(!slaw)
      error("No destination generator found: add module Slaw to the\
        network");
    std::cout << "Mobility state of walker " << getContainingNode(this)->getIndex() << "\n";
    slaw->initializeMobilityState(trip, areas, home);
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
}

void SlawMobility::setTargetPosition()
{
  if(nextMoveIsWait)
    nextChange = simTime() + slaw->computePauseTime();
  else{
    if(trip.empty()) {
      slaw->computeTrip(trip, areas, lastPosition);
      //slaw->computeTrip(trip, areas, home);
      //targetPosition = home;
    }
    //else
    targetPosition = slaw->LATP(lastPosition, trip);
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

void SlawMobility::saveTrip() {
  std::ofstream ofs("trip.txt");
  if(ofs.is_open()) {
    for (auto it = trip.begin(); it != trip.end(); it++)
      ofs << it->x << " " << it->y << "\n";
    ofs.close();
  }
  else
    std::cerr << "Trip couldn't be written" << '\n';
}
