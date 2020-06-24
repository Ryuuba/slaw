#include "UniformPauseTimeModel.h"

Define_Module(UniformPauseTimeModel);

void UniformPauseTimeModel::initialize(int stage) {
  a = par("par1").doubleValue();
  b = par("par2").doubleValue();
}

double UniformPauseTimeModel::computePauseTime() {
  Enter_Method_Silent();
  return uniform(a, b);
}