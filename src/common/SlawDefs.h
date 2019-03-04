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

#if !defined(SLAW_DEFS_H_)
#define SLAW_DEFS_H_

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <omnetpp.h>
#include "Coord.h"

#include "HashCoordinate.h"

/** @brief Randomizes the elements of a vector */
template<typename T>
std::vector<T>& randomizeVector(omnetpp::cRNG* rng, std::vector<T>& v) {
  for(unsigned i = v.size()-1; i > 0; --i) {
    unsigned rnd = omnetpp::uniform(rng, 0, 1) * i;
    std::swap(v[i], v[rnd]);
  }
  return v;
}

/** @brief List to store the unvisited waypoints integrating the trip of a 
 *  walker. A list is employed to efficiently remove a waypoint once LATP
 *  is inviked. */
typedef std::list<inet::Coord> WaypointList;

/** @brief Vector used to stored the confined areas of a walker. According to
 *  the SLAW's authors, this is denoted by C_k. */
typedef std::vector<unsigned> AreaSet;

/** @brief Vector used to represent confined areas c_i. */
typedef std::vector<inet::Coord> Area;

/** @brief Enumeration mapping the possible pause time models this implamentation provides. The PARETO:BOUNDED is the most appropiate */
enum class PauseTimeModelType : uint8_t {
  CONSTANT = 1, UNIFORM = 2, NORMAL = 3, PARETO_BOUNDED = 4
};

/** @brief Enumeration mapping the speed models this implementation provides.
 *  For pedestrian motion, the normal model is the most appropiate. If it is
 *  suppose a transportation mean is used, then the Rhee's model is the best 
 *  option to configure the simulator. */
enum class SpeedModelType : uint8_t {
  CONSTANT = 1, UNIFORM = 2, NORMAL = 3, RHEE = 4
};


#endif /* SLAW_DEFS_H_ */