
#if !defined(CONNECTIVITY_OBSERVER_H)
#define CONNECTIVITY_OBSERVER_H 

#include <vector>
#include <list>
#include <unordered_map>
#include <cmath>
#include <iterator>

#include <omnetpp.h>
#include "PositionObserver.h"

class ConnectivityObserver : public PositionObserver {
protected:
  /** @brief The link lifetime table (lt) is a data structure keeping the time 
   * from which a relationship of neighbors begins. The lt is mapped in such a 
   * way that the indices of the vector correspond to node IDs and the value of 
   * the vector is an unordered map where the key represents the ID of a 
   * neighbor node and the value the time from which the relationship of 
   * neighbors begins. The intercontact time table (ictt) follows the same 
   * organization as the llt, but the ictt stores the time from which a 
   * neighbor node y leaves a neighborhood N(x), that is, y is not in N(x) */
  std::vector < std::unordered_map <unsigned, omnetpp::simtime_t> > llt, ictt;
  /** @brief These are signal carrying the statistics asociated to its namely */
  static omnetpp::simsignal_t linkLifetime, interContactTime;
  /** @brief The coverage radius of a node*/
  double radius;
  /** @brief The total of samples o be captured */
  unsigned ict_num, llt_num, ict_counter, llt_counter;
  omnetpp::simtime_t ict_min, llt_min;
protected:
  /** @brief Computes the one hop neighborhood of a node whose ID is passed as
   * an argument. Neighborhoods are computed using the nodeMap. This function
   * member also updates the lt and the ictt */
  virtual std::unordered_map<unsigned, omnetpp::simtime_t>
  computeOneHopNeighborhood(unsigned);
public:
  /** @brief Initializes data structures and data member */
  ConnectivityObserver();
  /** @brief initializes data structures and the value of radius */
  virtual void initialize() override;
  /** @brief Receives the quadrant of a module and updates its one-hop 
   * neighborhood */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*) override;
};

#endif // CONNECTIVITY_OBSERVER_H 
