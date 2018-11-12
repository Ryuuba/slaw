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

class FlightLengthObserver: public omnetpp::cSimpleModule,
public omnetpp::cListener {
protected:
  unsigned numOfSamples, counter;
  static omnetpp::simsignal_t intraFlight;
  static omnetpp::simsignal_t interFlight;
public:
  FlightLengthObserver();
  ~FlightLengthObserver();
  void initialize();
  void handleMessage(omnetpp::cMessage*);
  void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, double, 
    omnetpp::cObject*);
};

#endif