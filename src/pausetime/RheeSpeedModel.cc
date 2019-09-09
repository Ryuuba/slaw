#include <cmath>
#include "RheeSpeedModel.h"

Define_Module(RheeSpeedModel);

void RheeSpeedModel::initialize() {}

void RheeSpeedModel::rheeModel(double flightLength) {
    if(flightLength < 500.0)
        speed = flightLength/(30.55*pow(flightLength, 1-0.89));
    else
        speed = flightLength/(0.76*pow(flightLength, 1-0.28));
}

double RheeSpeedModel::computeSpeed(double par) {
  rheeModel(par);
  return speed;
}