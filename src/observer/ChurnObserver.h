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

#include "inet/mobility/base/MovingMobilityBase.h"
#include "../contract/IObserver.h"
#include "../map/SelfSimilarWaypointMap.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 point_2;

class ChurnObserver : public IObserver {
protected:
  /** @brief Timer to compute the adjacency matrix*/
  omnetpp::cMessage* timer;
  /** @brief The number of walker at the observation area */
  unsigned membership_size;
  /** @brief The number of arrivals per window */
  unsigned arrival_num;
  /** @brief The number of departures */
  unsigned departure_num;
  /** @brief The number of nodes each observation time */
  static omnetpp::simsignal_t membership_stat, arrival_stat, departure_stat;
  /** @brief the signal ID carriying the quadrant where a emitting node is 
   * located */
  static omnetpp::simsignal_t position;
protected:
  /** @brief Nodes at an observation area */
  std::set<unsigned> membership;
  /** @brief Convex hull of the observation area */
  const std::vector<point_2>* polygon;
protected:
  /** @brief Computes whether a node x is at observtion area */
  bool isInObservationArea(inet::Coord&);
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
  /** @brief Computes the adjacency matrix */
  virtual void handleMessage(omnetpp::cMessage*);
};

#endif