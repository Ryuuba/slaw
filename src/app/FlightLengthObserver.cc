#include "FlightLengthObserver.h"

Define_Module(FlightLengthObserver);

omnetpp::simsignal_t FlightLengthObserver::flight = registerSignal("flightLength");
omnetpp::simsignal_t FlightLengthObserver::intraFlight = registerSignal("intraFlightLength");
omnetpp::simsignal_t FlightLengthObserver::interFlight = registerSignal("interFlightLength");

FlightLengthObserver::FlightLengthObserver() : numOfSamples(0), counter(0) {
  getSimulation()->getSystemModule()->subscribe(flight, this);
  getSimulation()->getSystemModule()->subscribe(intraFlight, this);
  getSimulation()->getSystemModule()->subscribe(interFlight, this);
}

FlightLengthObserver::~FlightLengthObserver(){
  std::cout << "Simulation Flight Observer\n";
  if (isSubscribed(flight, this)) {
    unsubscribe(flight, this);
    std::cout << "flight: unsubscribe done\n";
  }
  if (isSubscribed(intraFlight, this)) {
    unsubscribe(intraFlight, this);
    std::cout << "intraflight: unsubscribe done\n";
  }
  if (isSubscribed(interFlight, this)) {
    unsubscribe(interFlight, this);
    std::cout << "interflight: unsubscribe done\n";
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
    std::cout << counter << " samples have been gathered\n";
    endSimulation();
  }
}