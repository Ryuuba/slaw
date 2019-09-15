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

#if !defined(SLAW_MATLAB_H)
#define SLAW_MATLAB_H

#include "../contract/ISlawTripManager.h"

class SlawMatlab : public ISlawTripManager {
protected:
  /** @brief values determing the fraction of confined areas and the portion of
   *  waypoints that integrates a destination list */
  double clusterRatio, waypointRatio;
protected:
  /** @brief Assigns the confined areas of all walkers. The areas are
   *  determined by their weight, which is computed by dividing their
   *  number of waypoints into the number of waypoints integrating the map */
  virtual void assignConfinedAreas();
  /** @brief Gets k points from a given area randomly. It's used for debugging
   * purposes */
  virtual void getWaypointChunkRandomly(WaypointList&, const Area*, unsigned);
  /** @brief Computes a new list of unvisited waypoints. The new trip does
   *  not have the last waypoint where the walker stayed. Otherwise, the
   *  LATP algorithm will crash.
   */
  virtual WaypointList computeDestinationList(
    const AreaSet&, inet::Coord& 
  ) override;
public:
  /** @brief Default constructor */
  SlawMatlab() : clusterRatio(0.0), waypointRatio(0.0) { }
  /** @brief Default destructor. */
  virtual ~SlawMatlab() { }
  /** @brief Initializes the parameters of this module from the omnetpp.ini 
   *  configuration file*/
  virtual void initialize(int stage) override;
  virtual int numInitStages() const override {return 3;}
  /** @brief Sets the set of confined areas C_k and the initial waypoint 
   *  from wich the walker starts its trip. */
  virtual void setWalkerState(
    unsigned, AreaSet&, WaypointList&, inet::Coord&
  );
  virtual inet::Coord getNextDestination(
    WaypointList&, const AreaSet&, inet::Coord&, unsigned
  ) override;
  virtual double getPauseTime() override {
    return pause_time->computePauseTime();
  }
  virtual double getSpeed() override {
    return speed->computeSpeed();
  }
};


#endif /* SLAW_MATLAB_ */