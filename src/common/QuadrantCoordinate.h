#include <omnetpp.h>

// This struct inheritates of omnetpp::cObject to be emitable 
struct QuadrantCoordinate : public omnetpp::cObject{
  /* @brief A quadrant 'q' is a number resulting of dividing a square area into 
  * squares of side sideLength. A subquadrant 'subq' is the result of 
  * dividing a quadrant into four subquadrants */
  unsigned q, subq;
  /* @brief The xy coordinates of a node which are into q, sub_q */
  double x, y;
  //Quadrants are constructed with an invalid value
  QuadrantCoordinate(): q(0), subq(0), x(0.0), y(0.0) {};
  bool operator == (const QuadrantCoordinate& rhs) {
    return (q == rhs.q) && (subq == rhs.subq);
  }
  bool operator != (const QuadrantCoordinate& rhs) {
    return (q != rhs.q) || (subq != rhs.subq);
  }
};