#if !defined(GLOBAL_CONNECTIVITY_OBSERVER)
#define GLOBAL_CONNECTIVITY_OBSERVER

#include <algorithm>
#include <vector>
#include <list>
#include <fstream>
#include <set>
#include <string>
#include <omnetpp.h>

#include "PositionObserver.h"
#include "../map/SelfSimilarWaypointMap.h"
#include "../common/AdjacencyMatrix.h"


class GlobalConnectivityObserver : public PositionObserver {
protected:
  /** @brief Timer to compute the adjacency matrix*/
  omnetpp::cMessage* msg;
  /** @brief File where the adjacency matrix will be stored */
  const char* filename;
  /** @brief The signal carrying the degree per node */
  static omnetpp::simsignal_t degree;
protected:
  /** @brief Data structures storing link lifetimes */
  AdjacencyMatrix adjacency_matrix;
  /** @brief Data structure storing N(x) */
  std::vector< std::list<unsigned> >* neighborhood_list;
protected:
  /** @brief Computes the one-hop neighborhood of a node x being at observation area */
  std::list<unsigned> computeOneHopNeighborhood(unsigned);
  /** @brief Computes new neighbors*/
  void computeNewNeighbors(unsigned id, std::list<unsigned>&);
  /** @brief Computes old neighbors*/
  void computeOldNeighbors(unsigned id, std::list<unsigned>&);
public:
  /** @brief default constructor */
  GlobalConnectivityObserver();
  /** @brief default destructor  */
  ~GlobalConnectivityObserver();
  /** @brief Receives the quadrant of a module and updates its one-hop 
   * neighborhood */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*) override;
  /** @brief Initializes the attributes of this class */
  virtual void initialize(int stage) override;
  /** @brief Computes the adjacency matrix */
  virtual void handleMessage(omnetpp::cMessage*);
  /** @brief Writes the adjacency matrix of the system */
  virtual void finish() override;
};

#endif