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

#include "SpeedModel.h"

void SpeedModel::setModel(
  omnetpp::cRNG* randgen, SpeedModelType type, double a, double b
) {
  rng = randgen;
  modelType = type;
  par.first = a;
  par.second = b;
}

double SpeedModel::rheeModel(double flightLength) {
    double speed;
    if(flightLength < 500.0)
        speed = flightLength/(30.55*pow(flightLength, 1-0.89));
    else
        speed = flightLength/(0.76*pow(flightLength, 1-0.28));
    return speed;
}

double SpeedModel::computeSpeed(double flightLength) {
  double speed;
  switch(modelType) {
    case SpeedModelType::CONSTANT:
      speed = par.first;
      break;
    case SpeedModelType::UNIFORM:
        speed = omnetpp::uniform(rng, par.first, par.second);
        break;
    case SpeedModelType::NORMAL:
        speed = omnetpp::truncnormal(rng, par.first, par.second);
        break;
    default:
        speed = rheeModel(flightLength);
        break;
  }
  return speed;
}

SpeedModel& SpeedModel::operator=(SpeedModel&& model) {
  if (this != &model) {
    rng = model.rng;
    modelType = model.modelType;
    par = std::move(model.par);
  }
}
