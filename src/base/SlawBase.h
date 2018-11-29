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

#ifndef SLAWBASE_H_
#define SLAWBASE_H_

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


#include <omnetpp.h>

#include "SelfsimilarWaypointMap.h"
#include "Speed.h"
#include "PauseTime.h"

template<typename T>
std::vector<T>& randomizeVector(omnetpp::cRNG* rng,std::vector<T>& v) {
  for(unsigned i = v.size()-1; i > 0; --i) {
    unsigned rnd = omnetpp::uniform(rng, 0, 1)*i;
    //unsigned rnd = omnetpp::intuniform(rng, 0, i);
    std::swap(v[i],v[rnd]);
  }
  return v;
}

/** @brief List to store the waypoints integrating the trip of a walker. A list
 *  is employed to store represents the trip due to the fact that it is
 *  necessary to remove a waypoint once LATP used it as next destination. */
typedef std::list<inet::Coord> Trip;

/** @brief Vector used to stored the confined areas of a walker. According to
 *  the authors' SLAW, this is denoted by C_k. */
typedef std::vector<unsigned> areaSet;

class SlawBase : public omnetpp::cSimpleModule{
protected:
  bool initialize;
  SelfsimilarWaypointMap* map;
  Speed* speed;
  PauseTime* pausetime;
  /** @brief Loads cluster list C_k from a file. This method is used for
   * debugging purposes
  */
  virtual void loadClusterList(char const*, unsigned, areaSet&);
  /** @brief Computes a cluster list according the Matlab implementation */
  virtual void computeClusterList(areaSet&);
  /** @brief Computes a walker trip according to the Matlab implementation */
  virtual void computeSlawTrip(Trip&, const areaSet&, inet::Coord&);
  /** @brief Adds the 10 percent of waypoints of a random-elected
   *  confined area to the trip of a walker */
  virtual void computeTripRandomness(Trip&, const areaSet&);
  /** @brief Adds waypoints to the trip of a walker until reaching a number
   *  between 120 and 150 */
  virtual void computeRutine(Trip&, const areaSet&, inet::Coord&);
  /** @brief Randomly, assigns a waypoint from a confined area. The area
   * vector is passed as formal parameter to do the assignment. */
  virtual inet::Coord computeHome(const areaSet&);
  /** @brief Computes the confined areas of a walker. The areas are
   *  determined by their weight, which is computed by dividing their
   *  number of waypoints into the number of waypoints integrating the map */
  virtual void computeConfinedAreas(areaSet&);
public:
  /** @brief Default constructor. It initializes the pointers map, speed, and
   *  pausetime with nullptr. */
  SlawBase();
  /** @brief Default destructor. */
  virtual ~SlawBase(){}
};

#endif /* SLAWBASE_H_ */
