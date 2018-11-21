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

#include "SelfsimilarWaypointMap.h"

//The input is the map name and the clustering radius
SelfsimilarWaypointMap::SelfsimilarWaypointMap(std::string& name, 
double radius, double H) : clusteringRadius(radius), mapName(name), hurstParameter(H){ 
  areaVector = new std::vector<Area>;
  weightIntVector = new std::vector<unsigned>;
  weightVector = new std::vector<double>;
  if(!loadAreaVector()) {
    std::cout << "Load selfsimilar waypoint map from " 
      << mapName << std::endl;
    std::list<inet::Coord> waypointList;
    if(loadMap(waypointList)) {
      //computeConfinedAreas(waypointList);
      makeWaypointCluster(waypointList);
      computeAreaWeights();
      saveAreaVector();
    }
    else std::cerr << "Map not loaded\n";
  }
}

SelfsimilarWaypointMap::~SelfsimilarWaypointMap() {
  std::cout << "Selfsimilar waypoint map\n";
  if(areaVector)
    delete areaVector;
  if(weightVector)
    delete weightVector;
  if(weightIntVector)
    delete weightIntVector;
}

bool SelfsimilarWaypointMap::loadMap(std::list<inet::Coord>& waypointList) {
  std::string filename(mapName + ".map");
  std::ifstream waypointFile(filename, std::ifstream::in);
  bool success = false;
  if(waypointFile.is_open()) {
    inet::Coord waypoint;
    while (!waypointFile.eof()) {
      waypointFile >> waypoint.x >> waypoint.y;
      waypointList.push_back(waypoint);
    }
    waypointList.pop_back();
    numberOfWaypoints = waypointList.size();
    std::cout << numberOfWaypoints << " waypoints have been read from "
      << filename << std::endl;
    waypointFile.close();
    success = true;
  }
  else std::cerr << "Erroneous filename, perhaps you added an extension\n";
  return success;
}

void SelfsimilarWaypointMap::computeConfinedAreas(std::list<inet::Coord>& 
waypointList) {
  std::cout << "Computing confined areas... it may take some minutes" 
            << std::endl;
  unsigned numOfWaypoints = 0;
  Area confinedArea; //confined area
  while(!waypointList.empty()) {
    inet::Coord popularWaypoint = getPopularWaypoint(waypointList);
    auto it = waypointList.begin();
    while(it != waypointList.end()) {
      if(popularWaypoint.distance(*it) <= clusteringRadius) {
        confinedArea.push_back(*it);
        numOfWaypoints++;
        waypointList.erase(it++);
      }
      else it++;
    }
    areaVector->push_back(confinedArea);
    std::cout << confinedArea.size() << " waypoints have been clustered\n";
    confinedArea.clear();
  }
  std::cout << numOfWaypoints << " waypoints have been clustered in "
    << areaVector->size() << " confined areas" << std::endl;
}

void SelfsimilarWaypointMap::makeWaypointCluster(std::list<inet::Coord>& waypointList) {
std::cout << "Clustering waypoints, it may take some minutes..." 
          << std::endl;
  unsigned numOfWaypoints = 0, areaID = 0;
  Area confinedArea; //confined area
  while(!waypointList.empty()) {
    areaIDMap[waypointList.front()] = areaID;
    confinedArea.push_back(waypointList.front());
    waypointList.pop_front();
    numOfWaypoints++;
    for (int i = 0; i < confinedArea.size(); i++){
      auto it = waypointList.begin();
      while(it != waypointList.end()) {
        if (confinedArea[i].distance(*it) <= clusteringRadius) {
          areaIDMap[*it] = areaID;
          confinedArea.push_back(*it);
          waypointList.erase(it++);
          numOfWaypoints++;
        }
        else
          it++;
      }
    }
    std::cout << confinedArea.size() << " waypoints have been clustered\n";
    areaVector->push_back(confinedArea);
    confinedArea.clear();
    areaID++;
  }
  std::cout << areaIDMap.size() << "waypoints match an areaID\n";
  std::cout << numOfWaypoints << " waypoints have been clustered in "
    << areaVector->size() << " confined areas" << std::endl;
}

inet::Coord SelfsimilarWaypointMap::
  getPopularWaypoint(std::list<inet::Coord>& waypointList) {
  inet::Coord popularWaypoint(0.0, 0.0);
  uint16_t maximum = 0;
  for(inet::Coord& w : waypointList) {
    uint16_t surroundingWaypoints = 0;
    for(inet::Coord& v : waypointList)
      if((w.distance(v) < clusteringRadius) && (w != v))
        surroundingWaypoints++;
    if(surroundingWaypoints >= maximum) {
      popularWaypoint = w;
      maximum = surroundingWaypoints;
    }
  }
  return popularWaypoint;
}

bool SelfsimilarWaypointMap::saveAreaVector() {
  bool result = false;
  std::string filename = mapName+"_cl"+std::to_string(int(clusteringRadius))+".dat";
  std::ofstream ofs(filename.c_str(), std::ofstream::out);
  if(ofs.is_open()) {
    for(auto& area : *areaVector) {
      for(inet::Coord& waypoint : area)
        ofs << waypoint.x << ' ' << waypoint.y << ' ';
      ofs << '\n';
    }
    ofs.close();
    result = true;
  }
  return result;
}

bool SelfsimilarWaypointMap::loadAreaVector() {
  bool result = false;
  std::string filename = mapName+"_cl"+std::to_string(int(clusteringRadius))+".dat";
  std::ifstream ifs(filename.c_str(), std::ifstream::in);
  unsigned areaID = 0;
  if(ifs.good()) {
    uint16_t waypointCounter = 0;
    inet::Coord waypoint;
    std::vector<inet::Coord> waypointCluster;
    std::string line;
    std::stringstream ss;
    while(std::getline(ifs, line)) { //Gets a line while they exist
      ss << line;
      while(ss >> waypoint.x >> waypoint.y) {
        waypointCounter++;
        waypointCluster.push_back(waypoint);
        areaIDMap[waypoint] = areaID;
      }
      areaVector->push_back(waypointCluster);
      waypointCluster.clear();
      line.clear();
      ss.clear();
      areaID++;
    }
    numberOfWaypoints = waypointCounter;
    computeAreaWeights();
    result = true;
    std::cout << areaVector->size()
      << " confined areas have been read" << std::endl;
  }
  return result;
}

void SelfsimilarWaypointMap::computeAreaWeights() {
  double weight = 0.0;
  weightVector->push_back(weight);
  for(auto& area : *areaVector) {
    weight += (double)area.size()/numberOfWaypoints;
    weightVector->push_back(weight);
  }
  for (unsigned i = 0; i < areaVector->size(); i++)
    for (unsigned j = 0; j < areaVector->at(i).size(); j++)
      weightIntVector->push_back(i);
}

const Area* SelfsimilarWaypointMap::getConfinedArea(unsigned index) {
  return static_cast<const Area*>(&areaVector->at(index));
}

const std::vector<double>* SelfsimilarWaypointMap::getAreaWeights() {
  return static_cast<const std::vector<double>*>(weightVector);
}

const std::vector<unsigned>* SelfsimilarWaypointMap::getIntAreaWeights() {
  return static_cast<const std::vector<unsigned>*>(weightIntVector);
}

int SelfsimilarWaypointMap::getAreaSize(unsigned index) {
  int size = -1;
  if (areaVector && (index < areaVector->size()))
    size = unsigned(areaVector->at(index).size());
  return size;
}

int SelfsimilarWaypointMap::getNumberOfAreas() {
  int size = -1;
  if (areaVector)
    size = int(areaVector->size());
  return size;
}

inet::Coord
SelfsimilarWaypointMap::getWaypoint(unsigned indexArea, unsigned index) {
  inet::Coord waypoint(-1.0, -1.0);
  if (indexArea < areaVector->size())
    if (index < areaVector->at(indexArea).size())
      waypoint = areaVector->at(indexArea).at(index);
  return waypoint;
}

void SelfsimilarWaypointMap::randomizeArea(omnetpp::cRNG* rng, unsigned area_index) {
    Area* area = &(areaVector->at(area_index));
    for(size_t i = area->size() - 1; i > 0; --i) {
    unsigned rnd = omnetpp::intuniform(rng, 0, i);
    std::swap(area->at(i),area->at(rnd));
  }
}

unsigned SelfsimilarWaypointMap::getAreaID(inet::Coord& c) {
  return areaIDMap[c];
}