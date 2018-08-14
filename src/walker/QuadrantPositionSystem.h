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

#ifndef QUADRANT_POSITION_SYSTEM_H
#define QUADRANT_POSITION_SYSTEM_H

#include <omnetpp.h>
#include "Coord.h"
#include "MovingMobilityBase.h"

#include "../common/QuadrantCoordinate.h"

class QuadrantPositionSystem : public omnetpp::cSimpleModule, 
  public omnetpp::cListener { 
protected:
  /* @biref The quadrant and subquadrant of the node holder */
  QuadrantCoordinate coordinate;
  /* @brief The number of quadrants on the x axis is denoted by x_num */
  unsigned x_num;
  /* @brief The length of quadrants */
  double sideLength;
  /* @brief The ID of the signal carrying the position of the node holder */
  static omnetpp::simsignal_t position;
  /* @brief The ID of the signal carrying the quadrant position of the node 
   * holder */
  static omnetpp::simsignal_t quadrant;
protected:
  /** @brief Computes the quadrant of the holding node according to its 
   * position */
  unsigned computeQuadrant();
  /** @brief Computes the subquadrant of the holding node according to its 
   * position */
  unsigned computeSubquadrant();
public:
  /** Constructs the protected member variables of this class */
  QuadrantPositionSystem();
  /** Unsubscribes this module of the module emitting the position of the holder
   *  node */
  ~QuadrantPositionSystem();
  /** Initializes the value the x_num and quadrantSize variable members */
  void initialize();
  /** This module doesn't receive messages */
  void handleMessage(omnetpp::cMessage*);
  /** @brief Receives the position of the node holding this module to compute 
   * the corresponding quadrant and subquadrant */
  void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, 
    omnetpp::cObject*, omnetpp::cObject*);
};

#endif