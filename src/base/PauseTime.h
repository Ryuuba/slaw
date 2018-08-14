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

#ifndef PAUSETIME_H_
#define PAUSETIME_H_

#include <tuple>
#include <cmath>
#include <cstdint>
#include <omnetpp.h>

enum class PauseTimeModel : uint8_t {
  CONSTANT = 1, UNIFORM = 2, NORMAL = 3, PARETO_BOUNDED = 4
};

class PauseTime {
protected:
  /** @brief The kind of model implemented by object of this class */
  PauseTimeModel model;
  /** @brief Tuple containing the parameters needed to configure the pausetime model*/
  std::tuple<double,double,double> param;
  /* @brief The random number generator used by the SLAW engine */
  omnetpp::cRNG* rng;
protected:
  /** @brief Method to compute the bounded Pareto distribution with parameters
   * alpha, L, H, where alpha is the shape parameter, L is the scale
   * parameter, and H is the higher limit */
  double pareto_bounded(double, double, double);
public:
  /** @brief default constructor */
  PauseTime(omnetpp::cRNG*);
  /** @brief overloaded constructor, it initializes the parameters of a given
   *  pausetime model*/
  PauseTime(omnetpp::cRNG*, PauseTimeModel, double, double b = 0.0, double H = 0.0);
  /** @brief Sets the kind of pausetime model implemented by this class. */
  virtual void setPauseTimeModel(PauseTimeModel);
  /** @brief Sets the value of the constant pausetime model. */
  virtual void setConstantModel(double);
  /** @brief Sets the parameters of the heavy-tailed model. */
  virtual void setParetoBoundedModel(double, double, double);
  /** @brief Sets the mean and stddev of the normal model. */
  virtual void setNormalModel(double,double);
  /** @brief Sets the parameters of the uniform model. */
  virtual void setUniformModel(double,double);
  /** @brief Returns a time according to the configured pausetime model. */
  virtual double computePausetime();
};

#endif /* PAUSETIME_H_ */
