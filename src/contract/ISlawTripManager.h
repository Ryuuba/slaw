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
#include "../extra_model/SpeedModel.h"
#include "../extra_model/PauseTimeModel.h"
#include "../latp/LATP.h"

class SlawTripManager : public omnetpp::cSimpleModule {
protected:
  /** @brief the number of walkers in a simulation */
  unsigned walkerNum;
  /** @brief Temporal data structure to store the C_k set of each walker */
  std::vector<AreaSet> CkSet;
  /** @brief Planning degree */
  double planningDegree;
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
  virtual WaypointList computeDestinationList(
    const AreaSet&, inet::Coord&
  ) = 0;
  virtual void assignConfinedAreas() = 0;
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
