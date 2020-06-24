#if !defined(WAYPOINT_SIGNAL_H)
#define WAYPOINT_SIGNAL_H

#include <omnetpp.h>
#include "inet/common/geometry/common/Coord.h"

class WaypointSignal : public omnetpp::cObject, omnetpp::noncopyable  {
public:
  inet::Coord next_waypoint;
};

Register_Class(WaypointSignal);

#endif // WAYPOINT_SIGNAL_H
