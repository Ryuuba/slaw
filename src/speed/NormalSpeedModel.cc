#include "NormalSpeedModel.h"

Define_Module(NormalSpeedModel);

void NormalSpeedModel::initialize(int stage) {
  if (stage == 0) {
    mean = par("speedA").doubleValue();
    std_dev = par("speedB").doubleValue();
  }
}

double NormalSpeedModel::computeSpeed(double par) {
  return truncnormal(mean, std_dev);
}