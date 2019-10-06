#if !defined(POSITION_OBSERVER_H)
#define POSITION_OBSERVER_H

#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

#include <omnetpp.h>
#include "Coord.h"
#include "MovingMobilityBase.h"
#include "../contract/IObserver.h"

class PositionObserver: public IObserver{
protected:
  //TODO add statistic to measure the quadrant distribution
  /** @brief Data structure storing the position of nodes in a 
   * square position system, indices correspond to the square number and values 
   * are lists of the node IDs located in a square */
  std::vector< std::list<unsigned> > nodeMap;
  /**@ brief Data structures storing the square number of a node with index i */
  std::vector<inet::Coord> nodePosition;
  /** @brief the signal ID carriying the quadrant where a emitting node is 
   * located */
  static omnetpp::simsignal_t position;
  /** @brief The coverage radius of walkers in meters. It equals the length of 
   * the side of a square in a square position system. */
  double radius, x_length, y_length;
  /** @brief the number of nodes in the simulation */
  unsigned numOfNodes;
  /** @brief the number of squares on an axis */
  unsigned x_num, y_num;
  /** @brief the Id of the node that emitted a signal */
  unsigned nodeId;
protected:
  /** @brief Returns the number of square given a x, y coordinate */
  unsigned computeSquare(const inet::Coord&);
  /** Returns the nine neighboring squares corresponding to a given square */
  std::list<unsigned> computeNeighboringSquares(unsigned);
public:
  /** Subscribes to signal quadrant and initializes the class attributes*/
  PositionObserver();
  /** Unsubscribes to signal quadrant */
  virtual ~PositionObserver();
  /** @brief Initializes the attributes of this class */
  virtual void initialize(int stage) override;
  /** @brief Return the number of stages */
  virtual int numInitStages() const {return 5;}
  /** This module does not receive messages */
  virtual void handleMessage(omnetpp::cMessage*);
  /** Receives the quadrant of a module and updates its one-hop neighborhod*/
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*);
};


#endif // POSITION_OBSERVER_H
