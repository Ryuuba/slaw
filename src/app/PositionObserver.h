
#if !defined(POSITION_OBSERVER_H)
#define POSITION_OBSERVER_H

#include <vector>
#include <list>
#include <algorithm>


#include <omnetpp.h>

#include "../common/QuadrantNotification.h"

class PositionObserver: public omnetpp::cSimpleModule,
  public omnetpp::cListener{
protected:
  //TODO add statistic to measure the quadrant distribution
  /** @brief Data structure storing the position of nodes in a 
   * quadrant system, indices are taken as the quadrant number and values are a 
   * list of nodes located in such quadrant */
  std::vector< std::list<unsigned> > nodeMap;
  /**@ brief Data structures storing the quadrant of a node with index i */
  std::vector<QuadrantCoordinate> nodePosition;
  /** @brief the signal ID carriying the quadrant where a emitting node is 
   * located */
  static omnetpp::simsignal_t quadrant;
  /** @brief the number of nodes in the simulation, the number of quadrants on
   * x-axis, the number of quadrants in y-axis and the id of the node emisor */
  unsigned numOfNodes, x_num, y_num, nodeId;
  /** @brief The current quadrant and the last quadrant of the node emisor */
  QuadrantCoordinate currentQuadrant, lastQuadrant;
protected:
  /** Returns the neighboring quadrants corresponding to a pair <q, sub_q>*/
  std::list<unsigned> computeNeighboringQuadrants(unsigned, unsigned);
public:
  /** Subscribes to signal quadrant and initializes the class attributes*/
  PositionObserver();
  /** Unsubscribes to signal quadrant */
  virtual ~PositionObserver();
  /** Initializes the attributes of this class */
  virtual void initialize();
  /** This module does not receive messages */
  virtual void handleMessage(omnetpp::cMessage*);
  /** Receives the quadrant of a module and updates its one-hop neighborhod*/
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*);
};


#endif // POSITION_OBSERVER_H
