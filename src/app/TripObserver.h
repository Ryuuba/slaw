#if !defined(TRIP_OBSERVER_H)
#define TRIP_OBSERVER_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <omnetpp.h>
#include "../contract/IObserver.h"
#include "Coord.h"
#include "../signals/WaypointSignal.h"

class TripObserver: public IObserver
{
protected:
  /** @brief The number of observed trips */
  unsigned counter;
  /** @brief The number of trip to be recorded */
  unsigned sample_size;
  /** @brief Signal carrying the size of a trip a walker has completed */
  static omnetpp::simsignal_t trip_size;
  /** @brief Signal carrying the stats related to trips */
  static omnetpp::simsignal_t trip_size_stat;
  /** @brief Signal carrying the node position */
  static omnetpp::simsignal_t next_waypoint;
  /** @brief Signal carrying the node position */
  static omnetpp::simsignal_t next_waypoint_x, next_waypoint_y;
public:
  TripObserver();
  /** @brief Closes the file where the trip is written */
  ~TripObserver();
  /** @brief Reads from the configuration file the name of the output file */
  virtual void initialize(int stage) override;
  /** @brief Return the number of stages */
  virtual int numInitStages() const {return 5;}
  /** @brief This module does not receive messages */
  virtual void handleMessage(omnetpp::cMessage*);
  /** @brief Receives the number of trips a walker has completed */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*);
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    long, omnetpp::cObject*);
};

#endif