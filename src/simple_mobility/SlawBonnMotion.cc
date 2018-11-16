#include "SlawBonnMotion.h"

Define_Module(SlawBonnMotion);

omnetpp::simsignal_t
  SlawBonnMotion::flight = registerSignal("flight");

SlawBonnMotion::SlawBonnMotion() : pause(true) {}

void SlawBonnMotion::initialize(int state) {
  BonnMotionMobility::initialize(state);
  if (state == 0) {
    std::string filename(par("mapname").stdstringValue());
    loadMap(filename + ".map");
    WATCH(pause);
  }
}

void SlawBonnMotion::setInitialPosition() {
  BonnMotionMobility::setInitialPosition();
  lastWaypoint = lastPosition;
}

void SlawBonnMotion::setTargetPosition()
{
  BonnMotionMobility::setTargetPosition();
  if (lastPosition == targetPosition) {
      if (!pause) {
        double distance = targetPosition.distance(lastWaypoint);
        emit(flight, distance);
        lastWaypoint = targetPosition;
      }
      pause = true;
  }
  else
    pause = false;
}

bool SlawBonnMotion::loadMap(const std::string&& filename) {
  std::ifstream waypointFile(filename, std::ifstream::in);
  bool success = false;
  if(waypointFile.is_open()) {
    inet::Coord waypoint;
    while (!waypointFile.eof()) {
      waypointFile >> waypoint.x >> waypoint.y;
      waypointMap[waypoint] = true;
    }
    auto numberOfWaypoints = waypointMap.size();
    waypointFile.close();
    success = true;
  }
  else std::cerr << "Erroneous filename, perhaps you added an extension\n";
  return success;
}