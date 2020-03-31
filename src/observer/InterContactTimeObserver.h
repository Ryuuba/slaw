#if !defined(INTER_CONTACT_TIME_OBSERVER)
#define INTER_CONTACT_TIME_OBSERVER 

#include <vector>
#include <list>
#include <unordered_map>
#include <cmath>
#include <iterator>

#include <omnetpp.h>
#include "PositionObserver.h"

class InterContactTimeObserver : public PositionObserver {
protected:
  /** @brief The link lifetime table (llt) is a data structure keeping the time 
   * from which a relationship of neighbors begins. The llt is mapped in such a 
   * way that the indices of the vector correspond to node IDs and the value of 
   * the vector is an unordered map where the key represents the ID of a 
   * neighbor node and the value the time from which the relationship of 
   * neighbors begins. The intercontact time table (ictt) follows the same 
   * organization as the llt, but the ictt stores the time from which a 
   * neighbor node y leaves a neighborhood N(x), that is, y is not in N(x) */
  std::vector < std::unordered_map <unsigned, omnetpp::simtime_t> > llt, ictt;
  /** @brief These are signal carrying the statistics asociated to its namely */
  static omnetpp::simsignal_t linkLifetime, interContactTime;
  /** @brief The total of observations to be captured */
  unsigned ict_num, ict_counter, llt_counter;
  omnetpp::simtime_t ict_min, llt_min;
protected:
  /** @brief Computes the one hop neighborhood of a node whose ID is passed as
   * an argument. Neighborhoods are computed using the node_map. This function
   * member also updates the lt and the ictt */
  virtual std::unordered_map<unsigned, omnetpp::simtime_t>
  computeOneHopNeighborhood(unsigned);
public:
  /** @brief Initializes data structures and data member */
  InterContactTimeObserver();
  /** @brief Receives the quadrant of a module and updates its one-hop 
   * neighborhood */
  virtual void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t,  
    omnetpp::cObject*, omnetpp::cObject*) override;
  /** @brief Initializes the attributes of this class */
  virtual void initialize(int stage) override;
};

#endif // INTER_CONTACT_TIME_OBSERVER 
