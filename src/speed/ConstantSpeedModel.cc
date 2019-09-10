#include "ConstantSpeedModel.h"

Define_Module(ConstantSpeedModel);

void ConstantSpeedModel::initialize() {
  speed = par("speed").doubleValue();
}

double ConstantSpeedModel::computeSpeed(double par) {
  return speed;
}  