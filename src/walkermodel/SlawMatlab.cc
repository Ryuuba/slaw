#include "SlawMatlab.h"

Define_Module(SlawMatlab);

void SlawMatlab::initialize(int stage) {
  walker_model = par("walkerModel").stringValue();
  walkerNum = par("numOfWalker");
  clusterRatio = par("clusterRatio");
  waypointRatio = par("waypointRatio");
  planningDegree = par("planningDegree").doubleValue();
  latp.setLATP(planningDegree, getRNG(0));
  setMap();
  std::string filename(par("clusterList").stringValue());
  if (filename.compare("") != 0)
    loadCKFile(filename.c_str());
  else
    assignConfinedAreas();
}

void SlawMatlab::setWalkerState(
  unsigned walkerId, AreaSet& C_k, WaypointList& L, inet::Coord& initialWaypoint
) {
  C_k = std::move(CkSet[walkerId]);
  inet::Coord temp(-1.0, -1.0);
  L = computeDestinationList(C_k, temp);
  unsigned index = intuniform(0, L.size()-1);
  auto initialWaypointIt = L.begin();
  std::advance(initialWaypointIt, ceil(uniform(0,1) * L.size()) - 1);
  initialWaypoint = *initialWaypointIt;
  L.erase(initialWaypointIt);
}

WaypointList SlawMatlab::computeDestinationList(
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
    double aaa = double(area->size()) / waypointRatio;
    if (aaa < 1) {
      unsigned waypoint_id(ceil(uniform(0,1) * area->size()) - 1);
      uwl.push_back(area->at(waypoint_id));
    }
    else{
      double aaa_fraction, aaa_int;
      aaa_fraction = modf(aaa, &aaa_int);
      if (uniform(0,1) < aaa_fraction)
        getWaypointChunkRandomly(uwl, area, aaa_int+1);
      else if (aaa_int > 0)
        getWaypointChunkRandomly(uwl, area, aaa_int);
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
  unsigned portion = ceil(map->getNumberOfAreas() / clusterRatio);
  const std::vector<unsigned>* weights = map->getAreaWeights();
  for (unsigned i = 0; i < walkerNum; i++) {
    AreaSet C_k;
    unsigned j = 0; //number of inserted areas
    while (C_k.size() <= portion) {
      index = ceil(uniform(0,1) * weights->size())-1;
      unsigned clusterID((*weights)[index]);
      auto it = std::find(C_k.begin(), C_k.end(), clusterID);
      if (it == C_k.end())
        C_k.push_back(clusterID);
    }
    CkSet.push_back(std::move(C_k));
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

void SlawMatlab::getWaypointChunkRandomly(
  WaypointList& wList, const Area* area, unsigned k
) {
  unsigned i = 0;
  while (i < k) {
    auto rand_waypoint = area->at(uniform(0,1)*ceil(area->size()) - 1);
    auto it = std::find(wList.begin(), wList.end(), rand_waypoint);
    if (it == wList.end()) {
      wList.push_back(rand_waypoint);
      i++;
    }
  }
}
