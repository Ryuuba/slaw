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

#ifndef SPEED_H_
#define SPEED_H_

#include <cstdint>
#include <cmath>
#include <utility>
#include <omnetpp.h>

#include "../common/SlawDefs.h"

class SpeedModel {
protected:
  /* @brief The kind of model implemented by objects of this class */
  SpeedModelType modelType;
  /* @brief Tuple containing the parameters needed to configure the speed model. The values */
  std::pair<double, double> par;
  /* @brief The random number generator used by the SLAW engine */
  omnetpp::cRNG* rng;
protected:
    /** @brief Returns a speed according to the levy-walk speed model from Rhee,
     * et al. "Levy-walk nature of human mobility", TON, 2011 */
    double rheeModel(double);
public:
    /** @brief Overload constructor, receives the speed model and the parameters
     * needed to configure the speed model. Note that the Rhee mobility model
     * does not need any parameter*/
    virtual void setModel(
      omnetpp::cRNG*, SpeedModelType, double a = 0.0, double b = 0.0
    );
    /** @brief Returns the speed value according to a mobility model different
     * form the Rhee's*/
    virtual double computeSpeed(double flightLength = 0.0);
    /** @brief Move assigment operator */
    SpeedModel& operator=(SpeedModel&&);
};


#endif /* SPEED_H_ */
