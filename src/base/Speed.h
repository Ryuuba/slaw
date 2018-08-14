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

enum class SpeedModel : uint8_t {
    CONSTANT = 1, UNIFORM = 2, NORMAL = 3, RHEE = 4
};

class Speed {
protected:
  /* @brief The kind of model implemented by objects of this class */
  SpeedModel model;
  /* @brief Tuple containing the parameters needed to configure the speed model. The values */
  std::pair<double, double> speedPar;
  /* @brief The random number generator used by the SLAW engine */
  omnetpp::cRNG* rng;
protected:
    /** @brief Returns a speed according to the levy-walk speed model from Rhee,
     * et al. "Levy-walk nature of human mobility", TON, 2011 */
    double rheeModel(double);
public:
    /** @brief Default constructor */
    Speed(omnetpp::cRNG*);
    /** @brief Overload constructor, receives the speed model and the parameters
     * needed to configure the speed model. Note that the Rhee mobility model
     * does not need any parameter*/
    Speed(omnetpp::cRNG*, SpeedModel, double a = 0.0, double b = 0.0);
    /** @brief Default destructor */
    virtual ~Speed() {}
    /** @brief Sets the speed model */
    virtual void setSpeedModel(SpeedModel);
    /** @brief Sets the speed of the constant model. */
    virtual void setConstantModel(double);
    /** @brief Sets the parameters of the uniform distribution for the uniform
     *  speed model. */
    virtual void setUniformModel(double,double);
    /** @brief Sets the mean and stddev of the normal model. */
    virtual void setNormalModel(double,double);
    /** @brief Returns the speed value according to a mobility model different
     * form the Rhee's*/
    virtual double computeSpeed(double flightLength = 0.0);
    /** @brief returns the speed model employed to compute the node speed */
    virtual SpeedModel getSpeedModel();
};


#endif /* SPEED_H_ */
