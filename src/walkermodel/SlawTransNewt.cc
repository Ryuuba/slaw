#include "SlawTransNetw.h"

Define_Module(SlawTransNetw);

void SlawTransNetw::initialize(int stage) {
  if (stage == 0) {
    walker_model = par("walkerModel").stringValue();
    walkerNum = par("numOfWalker");
  }
  else if (stage == 1) {
    map = (SelfSimilarWaypointMap*) this->getSimulation()->
      getSystemModule()->getSubmodule("tripmanager")->getSubmodule(par("mapModule").stringValue());
    if (!map)
      error("Invalid self-similar waypoint map module");
    std::string filename(par("clusterList").stringValue());
    if (filename.compare("") != 0)
      loadCKFile(filename.c_str());
    else
      assignConfinedAreas();
    computeHome();
    pause_time = (IPauseTimeModel*) this->getSimulation()->
      getSystemModule()->getSubmodule("tripmanager")->getSubmodule(par("pauseTimeModule").stringValue());
    if (!pause_time)
      error("Invalid pause-time module");
    speed = (ISpeedModel*) this->getSimulation()->
      getSystemModule()->getSubmodule("tripmanager")->getSubmodule(par("speedModule").stringValue());
    if (!speed)
      error("Invalid speed module");
    latp_algorithm = (LATPAlgorithm*) this->getSimulation()->
      getSystemModule()->getSubmodule("tripmanager")->getSubmodule(par("latpModule").stringValue());
    if (!latp_algorithm)
      error("Invalid LATPAlgorithm module");
  }
}

void SlawTransNetw::setWalkerState(
  unsigned walkerId, AreaSet& C_k, WaypointList& L, inet::Coord& initialWaypoint
) {
  Enter_Method_Silent();
  C_k = std::move(CkSet[walkerId]);
  initialWaypoint = homeList[walkerId];
  L = computeDestinationList(C_k, initialWaypoint);
}

WaypointList SlawTransNetw::computeDestinationList(
  const AreaSet& C_k, inet::Coord& home
) {
  WaypointList uwl; //stands for unvisited waypoint list (uwl)
  computeTripRandomness(uwl, C_k);
  computeRutine(uwl, C_k, home);
  return uwl;
}

void SlawTransNetw::computeHome() {
  unsigned areaIndex;
  unsigned waypointIndex;
  for (unsigned i = 0; i < walkerNum; i++) {
    areaIndex = ceil(uniform(0,1) * CkSet[i].size()) - 1;
    waypointIndex = ceil(uniform(0,1) * map->getAreaSize(CkSet[i][areaIndex])) - 1;
    homeList.push_back(map->getWaypoint(CkSet[i][areaIndex], waypointIndex));
  }
}

void SlawTransNetw::computeTripRandomness(
  WaypointList& uwl, const AreaSet& C_k
) {
  AreaSet::const_iterator areaIt;
  unsigned rndAreaId;
  do {
    rndAreaId = intuniform(0, map->getNumberOfAreas()-1);
    areaIt = std::find(C_k.begin(), C_k.end(), rndAreaId);
  } while ( areaIt != C_k.end() );
  map->randomizeArea(getRNG(0), rndAreaId);
  auto area = map->getConfinedArea(rndAreaId);
  double portion = uniform(0.05,0.1);
  unsigned numOfRndWp = ceil(portion * area->size());
  uwl.insert(uwl.begin(), area->begin(), area->begin() + numOfRndWp);
}

void SlawTransNetw::computeRutine(
  WaypointList& uwl, const AreaSet& C_k, inet::Coord& home
) {
  AreaSet selectedAreas;
  Area temp;
  unsigned numberOfRutineAreas = unsigned(C_k.size());
  unsigned discartedAreaIndex = intuniform(0, numberOfRutineAreas - 1);
  unsigned tripSize = intuniform(120, 150) - 1;//minus the home waypoint
  for (unsigned areaIndex = 0; areaIndex < numberOfRutineAreas; areaIndex++)
    if (discartedAreaIndex != areaIndex)
      selectedAreas.push_back(C_k.at(areaIndex));
  for (auto& area : selectedAreas)
    for (auto& waypoint : *(map->getConfinedArea(area)))
      if (waypoint != home)
        temp.push_back(waypoint);
  unsigned rutineTripSize = tripSize - uwl.size();//uwl.size() equals rand wp
  if (temp.size() < rutineTripSize)
    for (auto& waypoint : temp)
      uwl.push_back(waypoint);
  else {
    randomizeVector(getRNG(0), temp);
    for (size_t i = 0; i < rutineTripSize; i++)
      uwl.push_back(temp[i]);
  }
}

void SlawTransNetw::assignConfinedAreas() {
  unsigned index, clusterID;
  const std::vector<unsigned>* weights = map->getAreaWeights();
  for (unsigned i = 0; i < walkerNum; i++) {
    unsigned confinedAreas = intuniform(3,5);
    AreaSet C_k;
    while (C_k.size() < confinedAreas) {
      index = ceil(uniform(0,1) * weights->size()) - 1;
      clusterID = (*weights)[index];
      auto it = std::find(C_k.begin(), C_k.end(), clusterID);
      if (it == C_k.end())
        C_k.push_back(clusterID);
    }
    CkSet.push_back(std::move(C_k));
  }
}

inet::Coord SlawTransNetw::getNextDestination(
    WaypointList& uwl, const AreaSet& C_k, inet::Coord& lastWaypoint, 
    unsigned walkerID
) {
  Enter_Method_Silent();
  inet::Coord nextWaypoint;
  if (uwl.empty()) {
    nextWaypoint = homeList[walkerID];
    uwl = std::move(computeDestinationList(C_k, homeList[walkerID]));
  }
  else
    nextWaypoint = (*latp_algorithm)(lastWaypoint, uwl);
  return nextWaypoint; 
}
