#include "NormalSpeedModel.h"

void NormalSpeedModel::initialize() {
  mean = par("mean").doubleValue();
  std_dev = par("stdDev").doubleValue();
}

double NormalSpeedModel::computeSpeed(double par) {
  return truncnormal(mean, std_dev);
}