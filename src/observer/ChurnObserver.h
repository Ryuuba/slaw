#if !defined(CHURN_OBSERVER_H)
#define CHURN_OBSERVER_H

#include <algorithm>
#include <vector>
#include <list>
#include <fstream>
#include <set>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <omnetpp.h>

#include "PositionObserver.h"
#include "../map/SelfSimilarWaypointMap.h"
#include "../common/AdjacencyMatrix.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 point_2;

class ChurnObserver : public PositionObserver {
protected:
  /** @brief Timer to compute the adjacency matrix*/
  omnetpp::cMessage* msg;
  /** @brief File where the adjacency matrix will be stored */
  const char* filename;
  /** @brief The minimum link lifetime to considered a link was established */
  unsigned llt_min;
  /** @brief The total number of observation to be captured */
  unsigned sample_size, membership_size;
  /** @brief The current number of observations */
  unsigned observation_counter;
  /** @brief The number of arrivals per window */
  unsigned arrival_num;
  /** @brief The number of departures */
  unsigned departure_num;
  /** @brief The number of nodes each observation time */
  static omnetpp::simsignal_t membership_stat, arrival_stat, departure_stat;
protected:
  /** @brief Nodes at an observation area */
  std::set<unsigned> membership;
  /** @brief Data structures storing link lifetimes */
  AdjacencyMatrix adjacency_matrix;
  /** @brief Data structure storing N(x) with pairs <id, time> */
  std::vector< std::list<unsigned> >* neighborhood_list;
  /** @brief Convex hull of the observation area */
  const std::vector<point_2>* polygon;
protected:
  /** @brief Computes the one-hop neighborhood of a node x being at observation area */
  std::list<unsigned> computeOneHopNeighborhood(unsigned);
  /** @brief Computes whether a node x is at observtion area */
  bool isInObservationArea(inet::Coord&);
  /** @brief Computes new neighbors*/
  void computeNewNeighbors(unsigned id, std::list<unsigned>&);
  /** @brief Computes old neighbors*/
  void computeOldNeighbors(unsigned id, std::list<unsigned>&);
public:
  /** @brief default constructor */
  ChurnObserver();
  /** @brief default destructor  */
  ~ChurnObserver();
  /** @brief Receives the quadrant of a module and updates its one-hop 
   * neighborhood */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*) override;
  /** @brief Initializes the attributes of this class */
  virtual void initialize(int stage) override;
  /** @brief Writes the adjacency matrix in a file */
  virtual void finish() override;
  /** @brief Computes the adjacency matrix */
  virtual void handleMessage(omnetpp::cMessage*);
};

#endif