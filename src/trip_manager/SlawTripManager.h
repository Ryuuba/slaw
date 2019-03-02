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

#if !defined(SLAW_TRIP_MANAGER_H)
#define SLAW_TRIP_MANAGER_H

#include "../common/SlawDefs.h"
#include "../map/SelfsimilarWaypointMap.h"
#include "../models/SpeedModel.h"
#include "../models/PauseTimeModel.h"
#include "../latp/LATP.h"

// TODO implement this questions in derived classes
// clusterListFile = par("clusterList").stringValue(); //Debug purposes
// readClusterListFromFile = (clusterListFile == "") ? false : true;
// cluster_ratio = par("cluster_ratio").doubleValue();
// waypoint_ratio = par("waypoint_ratio").doubleValue();
// void SlawEngine::initializeMobilityState(Trip& trip, areaSet& C_k, 
//   inet::Coord& home, unsigned walkerId) { //walkerID for debug purposes
//   if (readClusterListFromFile)
//   {
//     loadClusterList(clusterListFile.c_str(), walkerId, C_k);
//     std::cout << "SLAW Engine: Cluster list C_k is read from file\n";
//   }
//   else {
//     if (SLAW_MATLAB)
//       computeClusterList(C_k);
//     else
//       computeConfinedAreas(C_k); //Cookie algorithm
//   }
//   if (!SLAW_MATLAB) //Fixes the home waypoint according to the paper description
//     home = computeHome(C_k);
//   computeTrip(trip, C_k, home);
//   //computes home according to the SLAW Matlab implementation
//   if (SLAW_MATLAB) {
//     auto it_home = trip.begin();
//     std::advance(it_home, ceil(uniform(0,1)*trip.size()) - 1);
//     home = *it_home;
//     trip.erase(it_home);
//   }
// }

// void SlawEngine::computeTrip(Trip& walkerTrip, const areaSet& C_k, inet::Coord& home) {
//   if (SLAW_MATLAB)
//     computeSlawTrip(walkerTrip, C_k, home);
//   else
//   {
//     computeTripRandomness(walkerTrip, C_k);
//     computeRutine(walkerTrip, C_k, home);
//   }
// }

class SlawTripManager : public omnetpp::cSimpleModule {
protected:
  /** @brief the number of walkers in a simulation */
  unsigned walker_num;
  /** @brief Temporal data structure to store the C_k set of each walker */
  std::vector<AreaSet> C_k_Set;
  /** @brief Planning degree */
  double a;
  /** @brief The least-action trip-planning algorithm */
  LATPAlgorithm latp;
protected:
  /** @brief Loads a file containing the C_k set of each walker */
  void loadCKFile(char const*);
  /** @brief Initializes the self-similar waypoint map. */
  virtual void setMap();
  /** @brief Initializes the pausetime model */
  virtual void setPauseTimeModel();
  /** @brief Initializes the speed model */
  virtual void setSpeedModel();
  /** @brief Computes a new destination waypoint list according to the values of
   *  the member variable model. This member function is overridden */
  virtual WaypointList& computeDestinationList(
    const AreaSet&, inet::Coord&
  ) = 0;
public:
  /** @brief Object implementing the waypoint map */
  SelfsimilarWaypointMap* const map = new SelfsimilarWaypointMap;
  /** @brief Object implementing the speed model */ 
  SpeedModel* const speedModel = new SpeedModel;
  /** @brief Object implementing the pause-time model */
  PauseTimeModel* const pauseTimeModel = new PauseTimeModel;
public:
/** @brief Default destructor */
virtual ~SlawTripManager();
  /** @brief Sets the set of confined areas C_k, the initial set of unvisited 
   *  waypoints and the initial waypoint from which a walker starts its trip. 
   *  This member function must be overridden */
  virtual void setWalkerState(
    unsigned, AreaSet&, WaypointList&, inet::Coord&
  ) = 0;
  /** @brief Computes the next destination */
  virtual inet::Coord getNextDestination(
    WaypointList&, const AreaSet&, inet::Coord&, unsigned
  ) = 0;
  /** @brief Initializes the Trip Manager module. It must be overridden*/
  virtual void initialize() = 0;
};

#endif