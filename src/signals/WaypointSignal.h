#if !defined(WAYPOINT_SIGNAL_H)
#define WAYPOINT_SIGNAL_H

#include <omnetpp.h>
#include "Coord.h"

class WaypointSignal : public omnetpp::cObject, omnetpp::noncopyable  {
public:
  inet::Coord next_waypoint;
};

#endif // WAYPOINT_SIGNAL_H
