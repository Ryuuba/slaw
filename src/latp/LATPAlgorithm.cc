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

#include "LATPAlgorithm.h"

Define_Module(LATPAlgorithm);

void LATPAlgorithm::initialize(int stage) {
  if (stage == inet::INITSTAGE_LOCAL) {
    a = par("planningDegree").doubleValue();
  }
}

inet::Coord LATPAlgorithm::operator() (
  inet::Coord& currentPosition, WaypointList& unvisitedWaypointList
) {
  Enter_Method_Silent();
  std::vector<double> numerator_vec, cdf;
  double sum(0.0), cumulativeProbability(0.0);
  //Compute distance to each one of all unvisited waypoints, which are stored
  //in the trip walker
  for (auto& waypoint : unvisitedWaypointList) {
    double distance(currentPosition.distance(waypoint));
    double numerator(1.0/pow(distance, a));
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
  auto nextWaypointIt = unvisitedWaypointList.begin();
  std::advance(nextWaypointIt, index);
  inet::Coord nextWaypoint = *nextWaypointIt;
  unvisitedWaypointList.erase(nextWaypointIt);
  return nextWaypoint;
}
