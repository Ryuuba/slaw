#include "ConstantPauseTimeModel.h"

Define_Module(ConstantPauseTimeModel);

void ConstantPauseTimeModel::initialize(int stage) {
  pause_time = par("par1").doubleValue();
}

double ConstantPauseTimeModel::computePauseTime() {
  Enter_Method_Silent();
  return pause_time;
}