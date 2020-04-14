#include "SlawStationaryMobility.h"

Define_Module(SlawStationaryMobility);

void SlawStationaryMobility::initialize(int stage) {
  StationaryMobilityBase::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    map = (SelfSimilarWaypointMap*) this->getSimulation()->
      getSystemModule()->getSubmodule(par("map").stringValue());
    if (!map)
      error("Invalid self-similar waypoint map module");
  }
  else if (stage == inet::INITSTAGE_SINGLE_MOBILITY) {
    set_position(par("walkerModelType").stringValue());
  }
}

void SlawStationaryMobility::set_position(const char* model) {
  unsigned confined_areas, area_index, cluster_id, index = 0;
  auto cluster_ratio = par("clusterRatio").doubleValue();
  if (strcmp(model, "SlawMatlab") == 0) 
    confined_areas = ceil(map->getNumberOfAreas() / cluster_ratio) + 1;
  else if (strcmp(model, "SlawTransNetw") == 0) 
    confined_areas = intuniform(3, 5);
  else 
    error("SlawStationaryMobility: unknown model %s\n", model);
  std::vector<unsigned> C_k(confined_areas);
  const std::vector<unsigned>* weight_vector = map->getAreaWeights();
  std::cout << "vector weight size is " << weight_vector->size() << '\n';
  while (index < confined_areas) 
  {
    area_index = ceil(uniform(0,1) * weight_vector->size())-1;
    cluster_id = (*weight_vector)[area_index];
    auto it = std::find(C_k.begin(), C_k.end(), cluster_id);
    if (it == C_k.end())
      C_k[index++] = cluster_id;
  }
  std::cout << "C_k: ";
  for (auto&& id : C_k)
    std::cout << id << ' ';
  std::cout << '\n';
  auto random_area_index = intuniform(0, C_k.size()-1);
  std::cout << "Random area index: " << random_area_index << '\n';
  auto random_waypoint_index = intuniform(
    0, map->getAreaSize(C_k[random_area_index]-1)
  );
  std::cout << "random waypoint index is  " << random_waypoint_index << "\n";
  lastPosition = map->getWaypoint(
    C_k[random_area_index], random_waypoint_index
  );
  std::cout << "last position: " << lastPosition << '\n';
}
