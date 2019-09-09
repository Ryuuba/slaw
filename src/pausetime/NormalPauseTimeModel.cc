#include "NormalPauseTimeModel.h"

Define_Module(NormalPauseTimeModel);

void NormalPauseTimeModel::initialize() {
  mean = par("par1").doubleValue();
  std_dev = par("par2").doubleValue();
}

double NormalPauseTimeModel::computePauseTime() {
  return truncnormal(mean, std_dev);
}