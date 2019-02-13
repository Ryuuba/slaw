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

#ifndef SLAW_ENGINE_H_
#define SLAW_ENGINE_H_

#include "../base/SlawBase.h"

class SlawEngine: public SlawBase {
protected:
    /** @brief Computes flights according the SLAW_MATLAB implementation */
    bool SLAW_MATLAB;
    /** @brief Planning degree parameter. When it equals zero the next waypoint
     *  is chosen at random. Otherwise, it is very probable that LAPT returns a
     *  near waypoint to current one. */
    double a;
    bool isInitialized;
    /** @brief Boolean value to initialize the cluster list*/
    bool readClusterListFromFile;
protected:
    /** @brief Initializes the selfsimilar waypoint map. */
    virtual void initializeMap();
    /** @brief Initializes the pausetime model */
    virtual void initializePauseTimeModel();
    /** @brief Initializes the speed model */
    virtual void initializeSpeedModel();
public:
    /** @brief Default constructor. Initializes both derived and own
     *  parameters. */
    SlawEngine();
    /** @brief Default destructor. Destroys derived parameters: map, speed,
     *  and pausetime. */
    virtual ~SlawEngine();

    /** @brief Returns the state of the module. If it is not initialized, then
     *  the map, pausetime model and speed model are not initiliazed.*/
    virtual bool getStatus() { return isInitialized; }

    /** @brief Initializes the selfsimilar waypoint map, the speed model, and
     * the pausetime model. */
    virtual void initialize();
    /** @bried Initializes the mobility state of a walker. */
    virtual void initializeMobilityState(Trip&, areaSet&, inet::Coord&, unsigned);
    /** @brief Computes a walker trip. Receives the trip where waypoints will
     *  be stored and the vector storing the confined areas of the walker. */
    virtual void computeTrip(Trip&, const areaSet&, inet::Coord&);
    /** @brief An implementation of the least-action trip-planning algorithm.
     *  The first formal parameter is the walker's current position and the
     *  second one is the set containing the remaining waypoints of its trip.
     *  This method effectively erases the element in the trip that is returned
     *  as result. */
    inet::Coord LATP(inet::Coord&, Trip&);
    /** @brief Returns a pausetime for a walker according to the pausetime
     *  model that was read from the slaw.ned file. */
    virtual double computePauseTime();
    /** @brief Returns the arrival time from the first inet::coordinate to last one.
     *  The time is computed according the speed model read from the
     *  slaw.ned file. */
    //virtual double computeArrivalTime(const inet::Coord&, const inet::Coord&);
    /** @brief Draws a value from the speed model */
    virtual double getSpeed(double);
    /* @brief Returs true when a the areaID of two waypoints is not equal*/
    virtual bool sameArea(inet::Coord&, inet::Coord&);
    /** @brief Returns the area Id of a given corrdinate */
    virtual unsigned getAreaID(inet::Coord&);
    /** @brief Returns true if the simulation is performed according to the
     * original SLAW traces generator by Lee et al. */
    virtual bool isSLAW_MATLAB();
};


#endif /* SLAW_H_ */
