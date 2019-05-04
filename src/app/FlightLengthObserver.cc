#include "FlightLengthObserver.h"

Define_Module(FlightLengthObserver);

omnetpp::simsignal_t FlightLengthObserver::flight = registerSignal("flight");
omnetpp::simsignal_t FlightLengthObserver::flight_stat =
  registerSignal("flight_stat");
omnetpp::simsignal_t FlightLengthObserver::intraFlightLength = 
  registerSignal("intraFlightLength");
omnetpp::simsignal_t FlightLengthObserver::intraFlightLength_stat = 
  registerSignal("intraFlightLength_stat");
omnetpp::simsignal_t FlightLengthObserver::interFlightLength = 
  registerSignal("interFlightLength");
omnetpp::simsignal_t FlightLengthObserver::interFlightLength_stat = 
  registerSignal("interFlightLength_stat");

FlightLengthObserver::FlightLengthObserver():
  numOfSamples(0), counter(0)
{
  getSimulation()->getSystemModule()->subscribe(flight, this);
  getSimulation()->getSystemModule()->subscribe(intraFlightLength, this);
  getSimulation()->getSystemModule()->subscribe(interFlightLength, this);
}

FlightLengthObserver::~FlightLengthObserver()
{
  std::cout << "Simulation Flight Observer\n";
  if (isSubscribed(flight, this)) {
    unsubscribe(flight, this);
    std::cout << "flight: unsubscribe done\n";
  }
  if (isSubscribed(intraFlightLength, this)) {
    unsubscribe(intraFlightLength, this);
    std::cout << "intraflight: unsubscribe done\n";
  }
  if (isSubscribed(interFlightLength, this)) {
    unsubscribe(interFlightLength, this);
    std::cout << "interflight: unsubscribe done\n";
  }
}

void FlightLengthObserver::initialize()
{
  numOfSamples = par("numOfSamples");
  classifyFlight = par("classifyFlight").boolValue();
  std::cout << "Number of samples: " << numOfSamples << '\n';
  WATCH(counter);
}

void FlightLengthObserver::handleMessage(omnetpp::cMessage* msg)
{
  if (!msg->isSelfMessage())
    EV << "Simulation Observer: This module does not receive any messages!\n";
}

void FlightLengthObserver::receiveSignal(
  omnetpp::cComponent* src, omnetpp::simsignal_t id, double flightLength, 
  omnetpp::cObject* details
) {
  auto nodeId = dynamic_cast<omnetpp::cModule*>(src)->getParentModule()->getIndex();
  if (omnetpp::simTime() >= getSimulation()->getWarmupPeriod()) {
    counter++;
    processSignal(nodeId, id, flightLength);
    if (counter%10000 == 0)
      std::cout << "FlightLengthObserver: " << counter 
        << " samples have been produced\n";
  }
  if (counter == numOfSamples) {
    std::cout << "FlightLengthObserver: " << counter 
      << " samples have been gathered\n";
    endSimulation();
  }
}

void FlightLengthObserver::processSignal(
  int nodeId, omnetpp::simsignal_t id, double flightLength
) {
  if (classifyFlight) {
    if (id == intraFlightLength) {
      // std::cout << "New intra-flight is received " << flightLength 
      //   << " from " << nodeId << '\n';
      emit(intraFlightLength_stat, flightLength);
    }
    else if (id == interFlightLength) {
      // std::cout << "New inter-flight is received " << flightLength 
      //   << " from " << nodeId << '\n';
      emit(interFlightLength_stat, flightLength);
    }
  }
  else {
    EV_INFO << "New flight is received " << flightLength 
      << " from " << nodeId << '\n';
    emit(flight_stat, flightLength);
  }
}