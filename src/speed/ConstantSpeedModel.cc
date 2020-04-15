#include "ConstantSpeedModel.h"

Define_Module(ConstantSpeedModel);

void ConstantSpeedModel::initialize(int stage) {
  if (stage == 0)
    speed = par("speed").doubleValue();
}

double ConstantSpeedModel::computeSpeed(double par) {
  Enter_Method_Silent();
  return speed;
}  