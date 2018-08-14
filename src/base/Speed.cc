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

#include "Speed.h"

Speed::Speed(omnetpp::cRNG* rng) : rng(rng) {}

Speed::Speed(omnetpp::cRNG* rng, SpeedModel m, double a, double b) : 
  rng(rng), model(m), speedPar(a,b) {}

double Speed::rheeModel(double flightLength) {
    double speed;
    if(flightLength < 500.0)
        speed = flightLength/(30.55*pow(flightLength, 1-0.89));
    else
        speed = flightLength/(0.76*pow(flightLength, 1-0.28));
    return speed;
}

void Speed::setSpeedModel(SpeedModel m) {
    model = m;
}

void Speed::setConstantModel(double a) {
    speedPar.first = a;
    speedPar.second = 0.0;
}

void Speed::setUniformModel(double a, double b) {
    speedPar.first = a;
    speedPar.second = b;
}

void Speed::setNormalModel(double mean, double stddev) {
    speedPar.first = mean;
    speedPar.second = stddev;
}

double Speed::computeSpeed(double flightLength) {
  double speed;
  switch(model) {
    case SpeedModel::CONSTANT:
      speed = speedPar.first;
      break;
    case SpeedModel::UNIFORM:
        speed = omnetpp::uniform(rng, speedPar.first, speedPar.second);
        break;
    case SpeedModel::NORMAL:
        speed = omnetpp::truncnormal(rng, speedPar.first, speedPar.second);
        break;
    default:
        speed = rheeModel(flightLength);
        break;
  }
  return speed;
}

SpeedModel Speed::getSpeedModel() {
    return model;
}
