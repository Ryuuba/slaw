#include <omnetpp.h>
#include "QuadrantCoordinate.h"

class QuadrantNotification : public omnetpp::cObject, omnetpp::noncopyable {
  public:
    QuadrantCoordinate coordinate;
};

Register_Class(QuadrantNotification);