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

#if !defined(SELF_SIMILAR_WAYPOINT_MAP_H)
#define SELF_SIMILAR_WAYPOINT_MAP_H

#include <omnetpp.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/ch_eddy.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 point_2;

#include "../common/SlawDefs.h"

//TODO: Make this class a simple module showing waypoints and clusters
//TODO: Create self-similar waypoint maps
class SelfSimilarWaypointMap : public omnetpp::cSimpleModule {
protected:
  /** @brief The default location of the directory containing self-similar 
   *  waypoints maps */
  const char* map_path;
  /** @brief Parameter determining the selfsimilarity degree of a waypoint
   * map, where 0.5 < hurstParameter < 1.0 */
  double hurst_parameter;
  /** @brief The number of waypoints integrating a self-similar waypoint map*/
  uint16_t waypoint_number;
  /** @brief The radius, in meters, determining the min distance among of 
   *  clusters */
  double clustering_radius;
  /** @brief Name of the containing the set of waypoints */
  const char* map_name;
  /** @brief The ID of the area to be observed */
  int observation_area;
  /** @brief A pointer to the canvas of the network */
  omnetpp::cCanvas* simulation_canvas;
protected:
  /** @brief Vector storing vectors of waypoints (coordinates),
   * in this structure indices corresponds to the ID of each cluster.*/
  std::vector<Area>* area_vector;
  /** @brief Vector storing the weights of each confined area.*/
  std::vector<unsigned>* weight_vector;
  /** @brief Unordered map holding pairs <coord, areaID> */
  std::unordered_map<inet::Coord, unsigned> area_id_map;
  /** @brief Vector storing the convex hull of the observed area */
  std::vector<point_2> convexhull;
protected:
  /** @brief Loads a file containing waypoints distributed in a
   * self-similar manner and save the contain in a list*/
  virtual bool loadMap(WaypointList&);
  /** @brief Loads a file containing the confined areas of a map.*/
  virtual bool loadAreaVector();
  /** @brief Builts waypoint clusters, where a cluster is a set of points 
   * connected transitively and whose Euclidean distance is less than the 
   * clustering radius */
  virtual void computeConfinedAreas(WaypointList&);
  /** @brief Writes in a file the area vector.*/
  virtual bool saveAreaVector();
  /** @brief Computes the weight of each confined area in the cluster list.
   * The result is returned in a std::vector<double> pointer. According to
   * the SLAW mobility model, the weight of a confined area equals the ratio
   * between the number of waypoints in a given confined area divided by the
   * total number of waypoints in a map */
  virtual void computeAreaWeights();
  /** @brief Evaluates if there are repeated coordinates in the waypoint list */
  virtual bool testWaypointList(WaypointList);
  /** @brief Draws the self-similar waypoint map in the simulation canvas */
  virtual void drawMap();
  /** @brief Draws the convex hull of an observation area*/
  virtual void drawConvexHull();
public:
  SelfSimilarWaypointMap() { }
  virtual ~SelfSimilarWaypointMap();
  /** @brief Returns the number of stages needed to initialize a SLAW 
   *  simulation */
  virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
  /** @brief Initializes the parameters of this module from the omnetpp.ini 
   *  configuration file. It tries to load a file containing a set
   *  of waypoint clusters. If such file does not exist, then it loads waypoints
   *  from a file whose name is saved in mapName and then it creates clusters
   *  of waypoints that are saved in a hidden file. The first parameter of this
   *  method is the map name, the second one is the clustering radius, the
   *  third and the last one are the map dimension */
  virtual void initialize(int stage) override;
  /** @brief Creates a selfsimilar waypoint map */
  //virtual void createMap();
  virtual unsigned getNumberOfWaypoints(){ 
    Enter_Method_Silent();
    return waypoint_number;
  }
  /** @brief Returns the number of confined areas integrating the map */
  virtual int getNumberOfAreas();
  /** @brief Returns a pointer to the vector storing the waypoints comprising
   *  the confined area corresponding to the index given as formal parameter */
  virtual const Area* getConfinedArea(unsigned);
  /** @brief Returns the size of a given confined area. The formal parameter
   *  is the index corresponding to the area whose size is requested */
  virtual int getAreaSize(unsigned);
  /** @brief Returns the waypoint that corresponds to a given confined area.
   *  The first formal parameter is the index of the confined area containing
   *  the waypoint and the second parameter is the index of the waypoint */
  virtual inet::Coord getWaypoint(unsigned, unsigned);
  /** @brief Returns a pointer to the vector storing the weights of each
   *  confined area loaded from a map */
  //virtual const std::vector<double>* getAreaWeights();
  /** @brief Returns a vector with the weight of areas */
  virtual const std::vector<unsigned>* getAreaWeights();
  /** @brief Randomizes waypoints stored in a given confined area */
  virtual void randomizeArea(omnetpp::cRNG*, unsigned);
  /** @brief Returns the areaID matching a given waypoint */
  virtual unsigned getAreaID(inet::Coord&);
  /* @brief Returns true when a the areaID of two waypoints is not equal */
  virtual bool isSameArea(inet::Coord&, inet::Coord&);
  /** @brief Returns the map name **/
  virtual const char* getMapName() { return map_name; }
  /** @brief Returns the convex hull of the observation area */
  virtual const std::vector<point_2>* getConvexHull() {
    Enter_Method_Silent();
    const std::vector<point_2>* ch = &convexhull;
    return ch;
  }
};

#endif /* SelfSimilarWaypointMap_H_ */