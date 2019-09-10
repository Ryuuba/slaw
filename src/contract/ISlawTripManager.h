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

#if !defined(I_SLAW_TRIP_MANAGER)
#define I_SLAW_TRIP_MANAGER

#include "../common/SlawDefs.h"
#include "../map/SelfsimilarWaypointMap.h"
#include "../extra/SpeedModel.h"
#include "../contract/IPauseTimeModel.h"
#include "../latp/LATP.h"

class ISlawTripManager: public omnetpp::cSimpleModule{
protected:
  /** Object computing pause times */
  IPauseTimeModel* pause_time;
  /** @brief the number of walkers in a simulation */
  unsigned walkerNum;
  /** @brief Temporal data structure to store the C_k set of each walker */
  std::vector<AreaSet> CkSet;
  /** @brief Planning degree */
  double planningDegree;
  /** @brief The least-action trip-planning algorithm */
  LATPAlgorithm latp;
  /** @brief The name of the individual walker model */
  const char* walker_model;
protected:
  /** @brief Loads a file containing the C_k set of each walker */
  void loadCKFile(char const*);
  /** @brief Initializes the self-similar waypoint map. */
  virtual void setMap();
  /** @brief Initializes the pausetime model */
  //virtual void setPauseTimeModel();
  /** @brief Initializes the speed model */
  virtual void setSpeedModel();
  /** @brief Computes a new destination waypoint list according to the values of
   *  the member variable model. This member function is overridden */
  virtual WaypointList computeDestinationList(
    const AreaSet&, inet::Coord&
  ) = 0;
  /** @brief Assigns the confined areas of all walkers. It must be overridden */
  virtual void assignConfinedAreas() = 0;
public:
  /** @brief Object implementing the waypoint map */
  SelfsimilarWaypointMap* const map = new SelfsimilarWaypointMap;
  /** @brief Object implementing the speed model */ 
  SpeedModel* const speedModel = new SpeedModel;
public:
/** @brief Default destructor */
virtual ~ISlawTripManager();
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
  virtual void initialize(int stage) = 0;
  virtual int numInitStages() const = 0;
  /** @brief Returns the name of the individual walker model*/
  const char* getIndividualWalkerModelName() {return walker_model;}
  virtual double getPauseTime () = 0;
};

#endif