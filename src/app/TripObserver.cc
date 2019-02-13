#include "TripObserver.h"

Define_Module(TripObserver);

omnetpp::simsignal_t TripObserver::waypoint = registerSignal("next_waypoint");

omnetpp::simsignal_t TripObserver::counter = registerSignal("trip_counter");

TripObserver::TripObserver()
{
  getSimulation()->getSystemModule()->subscribe(waypoint, this);
  getSimulation()->getSystemModule()->subscribe(counter, this);
}

TripObserver::~TripObserver()
{
  if (ofs.is_open()) {
    ofs.close();
    std::cout << "Trip file was written in " << filename << '\n';
  }
  getSimulation()->getSystemModule()->unsubscribe(waypoint, this);
  getSimulation()->getSystemModule()->unsubscribe(counter, this);
}

void TripObserver::initialize()
{
  filename = par("filename").stringValue();
  trip_number = par("trip_number");
  ofs.open(filename, std::ofstream::out);
  if (!ofs.is_open()){
    error("File %s couldn't be opened\n", filename);
    endSimulation();
  }
  else
    ofs << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
}

void TripObserver::handleMessage(omnetpp::cMessage* msg)
{
  error("Trip Observer: This module does not receive messages\n");
}

void TripObserver::receiveSignal(omnetpp::cComponent* src, omnetpp::simsignal_t id, omnetpp::cObject* value, omnetpp::cObject* details)
{
  auto position = dynamic_cast<inet::Coord*>(value);
  ofs << position->x << ' ' << position->y << '\n';
}

void TripObserver::receiveSignal(omnetpp::cComponent* src, 
  omnetpp::simsignal_t id, long counter_trip, omnetpp::cObject* details)
{
  if (counter_trip == trip_number)
    endSimulation();
}