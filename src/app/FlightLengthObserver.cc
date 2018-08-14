#include "FlightLengthObserver.h"

Define_Module(FlightLengthObserver);

omnetpp::simsignal_t FlightLengthObserver::flightLengthId = registerSignal("flightLength");

FlightLengthObserver::FlightLengthObserver() : numOfSamples(0), counter(0) {
  getSimulation()->getSystemModule()->subscribe(flightLengthId, this);
}

FlightLengthObserver::~FlightLengthObserver(){
  std::cout << "Simulation Observer\n";
  if (isSubscribed(flightLengthId, this)) {
    unsubscribe(flightLengthId, this);
    std::cout << "FlightLengthObserver: unsubscribe done\n";
  }
}

void FlightLengthObserver::initialize(){
  numOfSamples = par("numOfSamples");
  std::cout << "Number of samples: " << numOfSamples << '\n';
  WATCH(counter);
}

void FlightLengthObserver::handleMessage(omnetpp::cMessage* msg) {
  if (!msg->isSelfMessage())
    EV << "Simulation Observer: This module does not receive any messages!\n";
}

void FlightLengthObserver::receiveSignal(omnetpp::cComponent* src, 
  omnetpp::simsignal_t id, double flightLength, omnetpp::cObject* details) {
  auto nodeId = dynamic_cast<omnetpp::cModule*>(src)->getParentModule()->getIndex();
  if (omnetpp::simTime() >= getSimulation()->getWarmupPeriod()) {
    counter++;
    EV_INFO << "New flight is received " << flightLength 
    << " from " << nodeId << '\n';
    if (counter%10000 == 0)
      std::cout << "Counter: " << counter << '\n';
  }
  if (counter == numOfSamples) {
    endSimulation();
    std::cout << counter << " samples have been gathered\n";
  }
}