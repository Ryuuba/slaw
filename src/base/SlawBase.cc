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

#include "SlawBase.h"

Register_Abstract_Class(SlawBase);

//FIXME The add walker method have to initialize the state vector
SlawBase::SlawBase() {
  map = nullptr;
  speed = nullptr;
  pausetime = nullptr;
}

void SlawBase::computeConfinedAreas(areaSet& areaVector) {
  const std::vector<double>* weights = map->getAreaWeights();
  double rnd;
  unsigned areaID = 0;
  //uint8_t confinedAreas = intuniform(3, 5);
  //A number not corresponding to the paper
  uint8_t confinedAreas = 14;
  while (areaVector.size() < confinedAreas) {
    rnd = uniform(0.0, 1.0);
    while (!((*weights)[areaID] < rnd && rnd <= (*weights)[areaID+1]))
      areaID++;
    auto it = std::find(areaVector.begin(), areaVector.end(), areaID);
    if (it == areaVector.end())
      areaVector.push_back(areaID);
    areaID = 0;
  }
}

inet::Coord SlawBase::computeHome(const areaSet& areaVector) {
  unsigned areaIndex = intuniform(0, areaVector.size() - 1);
  unsigned waypointIndex = intuniform(0, map->getAreaSize(areaVector[areaIndex]) - 1);
  inet::Coord home = map->getWaypoint(areaVector[areaIndex], waypointIndex);
  return home;
}

void SlawBase::computeTripRandomness(Trip& trip, const areaSet& C_k) {
  areaSet::const_iterator areaIt;
  unsigned rndAreaId;
  do {
    rndAreaId = intuniform(0, map->getNumberOfAreas()-1);
    areaIt = std::find(C_k.begin(), C_k.end(), rndAreaId);
  } while ( areaIt != C_k.end() );
  std::vector<inet::Coord> temp(*(map->getConfinedArea(rndAreaId)));
  randomizeVector(getRNG(0), temp);
  unsigned numOfRndWp = ceil(0.1 * temp.size());
  trip.insert(trip.begin(), temp.begin(), temp.begin()+numOfRndWp);
}

void
SlawBase::computeRutine(Trip& trip, const areaSet& rutineAreas, 
  inet::Coord& home) {
  areaSet selectedAreas;
  std::vector<inet::Coord> temp;
  unsigned numberOfRutineAreas = unsigned(rutineAreas.size());
  //index of discarded area
  unsigned rndAreaIndex = intuniform(0, numberOfRutineAreas - 1);
  unsigned tripSize = intuniform(120, 150) - 1;//minus the home waypoint
  for (unsigned areaIndex = 0; areaIndex < numberOfRutineAreas; areaIndex++)
    if (rndAreaIndex != areaIndex)
      selectedAreas.push_back(rutineAreas.at(areaIndex));

  for (auto& area : selectedAreas) {
    for (auto& waypoint : *(map->getConfinedArea(area)))
      if (waypoint != home)
        temp.push_back(waypoint);
  }

  unsigned rutineTripSize = tripSize - trip.size();//trip.size() equals rand wp
  if (temp.size() < rutineTripSize)
    for (auto& waypoint : temp)
      trip.push_back(waypoint);
  else {
    randomizeVector(getRNG(0), temp);
    for (int i = 0; i < rutineTripSize; i++)
      trip.push_back(temp[i]);
  }
}

