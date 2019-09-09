#include "ConstantPauseTimeModel.h"

Define_Module(ConstantPauseTimeModel);

void ConstantPauseTimeModel::initialize() {
  pause_time = par("par1").doubleValue();
}

double ConstantPauseTimeModel::computePauseTime() {
  return pause_time;
}