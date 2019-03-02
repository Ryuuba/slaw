#include "SlawMatlab.h"

Define_Module(SlawMatlab);

void SlawMatlab::initialize() {
  walker_num = par("numOfWalker");
  cluster_ratio = par("clusterRatio");
  waypoint_ratio = par("waypointRatio");
  a = par("planningDegree").doubleValue();
  latp.setLATP(a, getRNG(0));
  setMap();
  setPauseTimeModel();
  setSpeedModel();
  std::string filename(par("filename").stringValue());
  if (filename.compare("") != 0)
    loadCKFile(filename.c_str());
  else
    assignConfinedAreas();
}

void SlawMatlab::setWalkerState(
  unsigned walkerId, AreaSet& C_k, WaypointList& L, inet::Coord& initialWaypoint
) {
  C_k = std::move(C_k_Set[walkerId]);
  inet::Coord temp(-1.0, -1.0);
  L = computeDestinationList(C_k, temp);
  unsigned index = intuniform(0, L.size()-1);
  auto initialWaypointIt = L.begin();
  std::advance(initialWaypointIt, ceil(uniform(0,1) * L.size()) - 1);
  initialWaypoint = *initialWaypointIt;
  L.erase(initialWaypointIt);
}

WaypointList& SlawMatlab::computeDestinationList(
  const AreaSet& C_k, inet::Coord& lastWaypoint
) {
  WaypointList uwl; //stands for unvisited waypoint list (uwl)
  AreaSet unvisitedAreas(C_k);
  unsigned randomAreaId; //Index of an random area
  do {
    randomAreaId = ceil(uniform(0,1)*map->getNumberOfAreas())-1;
  } while (
    std::find(
      unvisitedAreas.begin(), unvisitedAreas.end(), randomAreaId
    ) != unvisitedAreas.end()
  );
  unvisitedAreas[ceil(uniform(0,1) * unvisitedAreas.size()) - 1] = randomAreaId;
  for (auto& areaId : unvisitedAreas) {
    auto area = map->getConfinedArea(areaId);
    //aaa is an ugly variable name from the SLAW MATLAB trace generator
    double aaa = double(area->size()) / waypoint_ratio;
    if (aaa < 1) {
      unsigned waypoint_id(ceil(uniform(0,1) * area->size()) - 1);
      uwl.push_back(area->at(waypoint_id));
    }
    else{
      double aaa_fraction, aaa_int;
      aaa_fraction = modf(aaa, &aaa_int);
      getWaypointChunkRandomly(uwl, area, aaa_int+1);
      if (uniform(0,1) >= aaa_fraction)
        uwl.pop_back();
    }
  }
  auto lastWaypointIt = std::find(uwl.begin(), uwl.end(), lastWaypoint);
  if (lastWaypointIt != uwl.end())
    uwl.erase(lastWaypointIt);
  return uwl;
}

void SlawMatlab::assignConfinedAreas() {
  unsigned index;
  //The portion of areas per walker
  unsigned portion = ceil(map->getNumberOfAreas() / cluster_ratio);
  const std::vector<unsigned>* weights = map->getAreaWeights();
  for (unsigned i = 0; i < walker_num; i++) {
    AreaSet C_k;
    unsigned j = 0; //number of inserted areas
    while (j < portion) {
      index = ceil(uniform(0,1) * weights->size())-1;
      unsigned clusterID((*weights)[index]);
      auto it = std::find(C_k.begin(), C_k.end(), clusterID);
      if (it == C_k.end())
        C_k[j++] = clusterID;
    }
    C_k_Set.push_back(std::move(C_k));
  }
}

inet::Coord SlawMatlab::getNextDestination(
    WaypointList& uwl, const AreaSet& C_k, inet::Coord& lastWaypoint,
    unsigned id //This argument is not utilized in this member function
) {
  inet::Coord nextWaypoint;
  if (uwl.empty())
    uwl = std::move(computeDestinationList(C_k, lastWaypoint));
  nextWaypoint = latp(lastWaypoint, uwl);
  return nextWaypoint; 
}