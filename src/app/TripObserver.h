#if !defined(TRIP_OBSERVER_H)
#define TRIP_OBSERVER_H

#include <fstream>
#include <iostream>
#include <omnetpp.h>
#include <iomanip>
#include <cmath>
#include <limits>
#include "Coord.h"

class TripObserver: public omnetpp::cSimpleModule,
                    public omnetpp::cListener
{
protected:
  /** @brief The name of the output file containing the trip of a walker */
  const char* filename;
  /** @brief The number of trip to be recorded */
  unsigned trip_number;
  /** @brief The stream used to write the trip */
  std::ofstream ofs;
  /** @brief Signal carrying the node position */
  static omnetpp::simsignal_t waypoint;
  /** @brief Signal carrying the number of trips a walker has completed */
  static omnetpp::simsignal_t counter;
public:
  TripObserver();
  /** @brief Closes the file where the trip is written */
  ~TripObserver();
  /** @brief Reads from the configuration file the name of the output file */
  void initialize();
  /** @brief This module does not receive messages */
  virtual void handleMessage(omnetpp::cMessage*);
  /** @brief Receives node position to register it in a file */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*);
  /** @brief Receives the number of trips a walker has completed */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    long, omnetpp::cObject*);
};

#endif