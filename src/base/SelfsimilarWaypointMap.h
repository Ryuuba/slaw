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

#ifndef SELFSIMILARWAYPOINTMAP_H_
#define SELFSIMILARWAYPOINTMAP_H_

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <omnetpp.h>

#include "Coord.h"
#include "../common/HashCoordinate.h"

/** @brief typedef used to represent confined areas c_i. */
typedef std::vector<inet::Coord> Area;

//TODO: Make this class a simple module showing waypoints and clusters
class SelfsimilarWaypointMap {
protected:
    /** @brief Parameter determining the selfsimilarity degree of a waypoint
     * map, where 0.5 < hurstParameter < 1.0 */
    double hurstParameter;
    /** @brief Map dimensions */
    //double x_max, y_max;
    /** @brief The number of waypoints integrating a self-similar waypoint map*/
    uint16_t numberOfWaypoints;
    /** @brief The radius, in meters, determining the limits of clusters */
    double clusteringRadius;
    /** @brief Name of the containing the set of waypoints */
    std::string mapName;
    /** @brief Bool variable indicating if waypoints have to be displayed */
    //bool displayWaypoints;
protected:
    /** @brief Vector storing vectors of waypoints (coordinates),
     * in this structure indices corresponds to the ID of each cluster.*/
    std::vector<Area>* areaVector;
    /** @brief Vector storing the weights of each confined area.*/
    std::vector<double>* weightVector;
    /** @brief Unordered map holding pairs <coord, areaID> */
    std::unordered_map<inet::Coord, unsigned> areaIDMap;
protected:
    /** @brief Loads a filename containing waypoints distributed in a
     * self-similar manner and save the contain in a list*/
    virtual bool loadMap(std::list<inet::Coord>&);
    /** @brief Makes confined areas of radius 'clusterRadius'. In additon,
     *  it initializes the weightVector */
    virtual void computeConfinedAreas(std::list<inet::Coord>&);
    /** @brief Builts waypoint clusters, where a cluster is a set of points 
     * connected transitively and whose Euclidean distance is less than the 
     * clustering radius */
    virtual void makeWaypointCluster(std::list<inet::Coord>&);
    /** @brief Loads a file containing confined areas of a map.*/
    virtual bool loadAreaVector();
    /** @brief Returns a waypoint with the maximum surrounding waypoints within
     *  a radius clusteringRadius.*/
    virtual inet::Coord getPopularWaypoint(std::list<inet::Coord>&);
    /** @brief Writes in a file the area vector.*/
    virtual bool saveAreaVector();
    /** @brief Draw the waypoints contained in the clusterList data structure
     *  when simulations are conducted in the tkenv.*/
    //virtual void displayMap();
    /** @brief Computes the weight of each confined area in the cluster list.
     * The result is returned in a std::vector<double> pointer. According to
     * the SLAW mobility model, the weight of a confined area equals the ratio
     * between the number of waypoints in a given confined area divided by the
     * total number of waypoints in a map */
    virtual void computeAreaWeights();
public:
    SelfsimilarWaypointMap(){}
    /** @brief Overloaded constructor that tries to load a file containing a set
     * of waypoint clusters. If such file does not exist, then it loads waypoints
     * from a file whose name is saved in mapName and then it creates clusters
     * of waypoints that are saved in a hidden file. The first parameter of this
     * method is the map name, the second one is the clustering radius, the
     * third and the last one are the map dimension*/
    SelfsimilarWaypointMap(std::string&, double, double);
    virtual ~SelfsimilarWaypointMap();
    /** @brief Creates a selfsimilar waypoint map.*/
    //virtual void createMap();
    virtual unsigned getNumberOfWaypoints(){ return numberOfWaypoints;}
    /** @brief Returns the number of confined areas integrating the map. */
    virtual int getNumberOfAreas();
    /** @brief Returns a pointer to the vector storing the waypoints comprising
     *  the confined area corresponding to the index given as formal parameter*/
    virtual const Area* getConfinedArea(unsigned);
    /** @brief Returns the size of a given confined area. The formal parameter
     *  is the index corresponding to the area whose size is requested. */
    virtual int getAreaSize(unsigned);
    /** @brief Returns the waypoint that corresponds to a given confined area.
     *  The first formal parameter is the index of the confined area containing
     *  the waypoint and the second parameter is the index of the waypoint. */
    virtual inet::Coord getWaypoint(unsigned, unsigned);
    /** @brief Returns a pointer to the vector storing the weights of each
     *  confined area loaded from a map. */
    virtual const std::vector<double>* getAreaWeights();
    /** @brief Randomizes waypoints stored in a given confined area */
    virtual void randomizeArea(omnetpp::cRNG*, unsigned);
    /** @brief Returns the areaID matching a given waypoint */
    virtual unsigned getAreaID(inet::Coord&);
};


#endif /* SELFSIMILARWAYPOINTMAP_H_ */
