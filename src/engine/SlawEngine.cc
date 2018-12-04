//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SlawEngine.h"

Define_Module(SlawEngine);

SlawEngine::SlawEngine() {
  map = nullptr;
  speed = nullptr;
  pausetime = nullptr;
  a = 0;
  isInitialized = false;
}

SlawEngine::~SlawEngine() {
  std::cout << "SLAW engine\n";
  if(map) {
    delete map;
    std::cout << "Destroying map: OK" << '\n';
  }
  if(speed) {
    delete speed;
    std::cout << "Destroying speed model: OK" << '\n';
  }
  if(pausetime) {
    delete pausetime;
    std::cout << "Destroying pausetime model: OK" << '\n';
  }
  std::cout << "SLAW module was destroyed" << '\n';
}

inet::Coord SlawEngine::LATP(inet::Coord& currentPosition, Trip& walkerTrip) {
  std::vector<double> numerator_vec, cdf;
  double sum(0.0), cumulativeProbability(0.0);
  //Compute distance to each one of all unvisited waypoints, which are stored
  //in the trip walker
  for (auto& waypoint : walkerTrip) {
    double distance(currentPosition.distance(waypoint));
    if (distance == 0.0) {
      std::cout << "Invalid waypoint: "<< waypoint << ' ' << currentPosition << "\n";
      endSimulation();
    }
    double numerator(1/pow(distance, a));
    sum += numerator;
    numerator_vec.push_back(numerator);
  }
  //Compute probability to move to all unvisited waypoints.
  for (auto& numerator : numerator_vec) {
    cumulativeProbability += numerator / sum;
    cdf.push_back(cumulativeProbability);
  }
  //Choose the next waypoint according to the probability vector
  double rnd = uniform(0, 1);
  auto it = std::upper_bound(cdf.begin(), cdf.end(), rnd);
  auto index = std::distance(cdf.begin(), it);
  auto nextWaypointIt = walkerTrip.begin();
  std::advance(nextWaypointIt, index);
  inet::Coord nextWaypoint = *nextWaypointIt;
  walkerTrip.erase(nextWaypointIt);
  return nextWaypoint;
}

void SlawEngine::initialize() {
  initializeMap();
  clusterListFile = par("clusterList").stringValue(); //Debug purposes
  initializePauseTimeModel();
  initializeSpeedModel();
  a = par("planningDegree").doubleValue();
  isInitialized = true;
  std::cout << "Slaw has been initialized\n";
}

void SlawEngine::initializeMap() {
  double hurstParameter = par("hurstParameter").doubleValue();
  std::string mapName(par("mapName").stringValue());
  double clusteringRadius = par("clusteringRadius");
  map = new SelfsimilarWaypointMap(mapName, clusteringRadius, hurstParameter);
}

void SlawEngine::initializePauseTimeModel() {
  PauseTimeModel pauseModel = 
    static_cast<PauseTimeModel>(uint8_t(par("pauseTimeModel")));
  double par1 = par("pausetimeA");
  double par2 = par("pausetimeB");
  double parH = par("pausetimeH");
  pausetime = new PauseTime(getRNG(0), pauseModel, par1, par2, parH);
}

void SlawEngine::initializeSpeedModel() {
  SpeedModel model = static_cast<SpeedModel>(uint8_t(par("speedModel")));
  double par1 = par("speedA");
  double par2 = par("speedB");
  speed = new Speed(getRNG(0), model, par1, par2);
}

void SlawEngine::initializeMobilityState(Trip& trip, areaSet& C_k, 
  inet::Coord& home, unsigned walkerId) { //walkerID for debug purposes
  //computeConfinedAreas(C_k);
  //computeClusterList(C_k); //debug purposes
  loadClusterList(clusterListFile.c_str(), walkerId, C_k);
  computeTrip(trip, C_k, home);
  //computes home according to the SLAW Matlab implementation
  auto it_home = trip.begin();
  std::advance(it_home, ceil(uniform(0,1)*trip.size()) - 1);
  home = *it_home;
  trip.erase(it_home);
}

void SlawEngine::computeTrip(Trip& walkerTrip, const areaSet& C_k, inet::Coord& home) {
  computeSlawTrip(walkerTrip, C_k, home);
  //computeTripRandomness(walkerTrip, C_k);
  //computeRutine(walkerTrip, C_k, home);
}

double SlawEngine::computePauseTime() {
  return pausetime->computePausetime();
}

// double SlawEngine::computeArrivalTime(const inet::Coord& w, const inet::Coord& v) {
//   double distance = w.distance(v);
//   emit(flightLength, distance);
//   return distance/speed->computeSpeed(distance);
// }

double SlawEngine::getSpeed(double distance){
  return speed->computeSpeed(distance);
}

bool SlawEngine::sameArea(inet::Coord& c1, inet::Coord& c2) {
  unsigned area1(map->getAreaID(c1));
  unsigned area2(map->getAreaID(c2));
  return area1 == area2;
}

unsigned SlawEngine::getAreaID(inet::Coord& c) {
  return map->getAreaID(c);
}