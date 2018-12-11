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
SlawBase::SlawBase() : initialize(false) {
  map = nullptr;
  speed = nullptr;
  pausetime = nullptr;
}

void SlawBase::computeClusterList(areaSet& clusterList) {
  unsigned index, ratio_cluster = 5;
  unsigned confinedAreas = ceil(map->getNumberOfAreas() / ratio_cluster);
  const std::vector<unsigned>* weights = map->getIntAreaWeights();
  while (clusterList.size() < confinedAreas) {
    index = ceil(uniform(0,1)*weights->size())-1;
    unsigned clusterID((*weights)[index]);
    auto it = std::find(clusterList.begin(), clusterList.end(), clusterID);
    if (it == clusterList.end())
      clusterList.push_back(clusterID);
  }
}

//TODO: Modify Slaw Engine and SlawMobility to load the cluster list of
//each walker with this member function
void SlawBase::loadClusterList(char const* filename, unsigned walkerID, areaSet& clusterList) {
  std::ifstream ifs(filename, std::ifstream::in);
  if (ifs.is_open()) {
    std::string c_k;
    unsigned linenumber = 0;
    while(std::getline(ifs, c_k)) {
        if (linenumber == walkerID) {
          std::istringstream iss(c_k);
          unsigned areaID;
          while (iss >> areaID)
            clusterList.push_back(areaID);
        }
        linenumber++;
    }
    if (clusterList.empty()) {
      std::cerr << "Slaw Base: walker ID " << walkerID << " does not match any area\n";
      endSimulation();
    }
    ifs.close();
  }
  else
    std::cerr << "SlawBase: " << filename << " couldn't be opened\n";
}

void SlawBase::computeConfinedAreas(areaSet& areaVector) {
  const std::vector<double>* weights = map->getAreaWeights();
  double rnd;
  unsigned areaID = 0, ratio_cluster = 5;
  //uint8_t confinedAreas = intuniform(3,5); //Slaw paper
  uint8_t confinedAreas = ceil(double(map->getNumberOfAreas()) / ratio_cluster);
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
  unsigned areaIndex = ceil(uniform(0,1)* areaVector.size()) - 1;
  unsigned waypointIndex = ceil(uniform(0,1) * map->getAreaSize(areaVector[areaIndex])) - 1;
  inet::Coord home = map->getWaypoint(areaVector[areaIndex], waypointIndex);
  return home;
}

//TODO implement replace cluster

void SlawBase::computeSlawTrip(Trip& trip, const areaSet& C_k, inet::Coord& home) {
  //Replaces randomly an area
  areaSet clusterList(C_k);
  unsigned ratio_cluster = 5;
  unsigned randomAreaId; //Value from the Slaw Matlab implementation
  do {
    //Index of random area
    randomAreaId = ceil(uniform(0,1)*map->getNumberOfAreas())-1;
  } while (find(clusterList.begin(), clusterList.end(), randomAreaId) != clusterList.end());
  clusterList[ceil(uniform(0,1)*clusterList.size())-1] = randomAreaId;
  for (auto& areaId : clusterList) {
    auto area = map->getConfinedArea(areaId);
    //aaa is an ugly variable name
    double aaa(double(area->size()) / ratio_cluster);
    if (aaa < 1) {
      unsigned waypoint_id(ceil(uniform(0,1)*area->size()) - 1);
      trip.push_back(area->at(waypoint_id));
    }
    else{
      double aaa_fraction, aaa_int;
      aaa_fraction = modf(aaa, &aaa_int);
      //map->randomizeArea(getRNG(0), areaId);
      if (uniform(0,1) < aaa_fraction) {
        //debug purposes
        //trip.insert(trip.end(), area->begin(), area->begin()+aaa_int+1);
        getWaypointChunkRandomly(trip, area, aaa_int+1);
      }
      else if (aaa_int > 0)
        getWaypointChunkRandomly(trip, area, aaa_int);
        //debug purposes
        //trip.insert(trip.end(), area->begin(), area->begin()+aaa_int);
    }
  }
  auto home_it = std::find(trip.begin(), trip.end(), home);
  if (home_it != trip.end())
    trip.erase(home_it);
}

void SlawBase::computeTripRandomness(Trip& trip, const areaSet& C_k) {
  areaSet::const_iterator areaIt;
  unsigned rndAreaId;
  do {
    rndAreaId = intuniform(0, map->getNumberOfAreas()-1);
    areaIt = std::find(C_k.begin(), C_k.end(), rndAreaId);
  } while ( areaIt != C_k.end() );
  map->randomizeArea(getRNG(0), rndAreaId);
  auto area = map->getConfinedArea(rndAreaId);
  unsigned numOfRndWp = ceil(0.1 * area->size());
  trip.insert(trip.begin(), area->begin(), area->begin()+numOfRndWp);
}

//TODO: make this method more efficient by means of randomizeArea()
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

void SlawBase::getWaypointChunkRandomly(Trip& trip, const Area* area, unsigned k) {
  unsigned i = 0;
  while (i < k) {
    auto rand_waypoint = area->at(uniform(0,1)*ceil(area->size()) - 1);
    auto it = std::find(trip.begin(), trip.end(), rand_waypoint);
    if (it == trip.end()) {
      trip.push_back(rand_waypoint);
      i++;
    }
  }
}