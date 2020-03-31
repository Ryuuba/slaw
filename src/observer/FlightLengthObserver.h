//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef FLIGHT_LENGTH_OBSERVER_H
#define FLIGHT_LENGTH_OBSERVER_H

#include <omnetpp.h>
#include <iostream>
#include "../contract/IObserver.h"

class FlightLengthObserver: public IObserver {
protected:
  unsigned numOfSamples, counter;
  bool classifyFlight;
  static omnetpp::simsignal_t flight, flight_stat;
  static omnetpp::simsignal_t intraFlightLength, intraFlightLength_stat;
  static omnetpp::simsignal_t interFlightLength, interFlightLength_stat;
  static omnetpp::simsignal_t remote_random_area, remote_random_area_stat;
protected:
  void processSignal(int, omnetpp::simsignal_t, double);
public:
  FlightLengthObserver();
  ~FlightLengthObserver();
  /** Initializes the attributes of this class */
  virtual void initialize(int stage) override;
  /** @brief Return the number of stages */
  virtual int numInitStages() const {return 5;}
  void handleMessage(omnetpp::cMessage*);
  void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, double, 
    omnetpp::cObject*);
  void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, bool, 
    omnetpp::cObject*);
};

#endif