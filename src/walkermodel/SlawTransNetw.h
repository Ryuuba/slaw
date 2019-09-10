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

#ifndef SLAW_IEEE_ACM_TON_H_
#define SLAW_IEEE_ACM_TON_H_

#include "../contract/ISlawTripManager.h"
#include "../common/SlawDefs.h"

class SlawTransNetw : public ISlawTripManager {
protected:
  /** @brief This data structure stores the home waypoint of all walkers */
  std::vector<inet::Coord> homeList;
protected:
  /** @brief Assigns the confined areas of all walkers. The areas are
   *  determined by their weight, and each walker has assigned three or five
   *  confined areas */
  virtual void assignConfinedAreas();
  /** @brief Adds the 5-10 percent of waypoints of a random-elected
   *  confined area to the trip of a walker */
  virtual void computeTripRandomness(WaypointList&, const AreaSet&);
  /** @brief Adds waypoints to the trip of a walker until reaching a number
   *  between 120 and 150 */
  virtual void computeRutine(WaypointList&, const AreaSet&, inet::Coord&);
  /** @brief Randomly, assigns a waypoint from a confined area to all walkers */
  virtual void computeHome();
  /** @brief Computes a new list of unvisited waypoints. The new trip does
   *  not have the last waypoint where the walker stayed. Otherwise, the
   *  LATP algorithm will crash.
   */
  virtual WaypointList computeDestinationList(
    const AreaSet&, inet::Coord& 
  ) override;
public:
  /** @brief Initializes the parameters of this module from the omnetpp.ini 
   *  configuration file*/
  virtual void initialize(int stage) override;
  virtual int numInitStages() const override {return 5;}
  /** @brief Sets the set of confined areas C_k and the initial waypoint 
   *  from wich the walker starts its trip. */
  virtual void setWalkerState(
    unsigned, AreaSet&, WaypointList&, inet::Coord&
  );
  /** @brief Computes the next destination according to a given walker state */
  virtual inet::Coord getNextDestination(
    WaypointList&, const AreaSet&, inet::Coord&, unsigned
  ) override;
  virtual double getPauseTime() override {
    return pause_time->computePauseTime();
  }
};

#endif /* SLAW_IEEE_ACM_TON_ */
