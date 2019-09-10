#include "UniformPauseTimeModel.h"

Define_Module(UniformPauseTimeModel);

void UniformPauseTimeModel::initialize(int stage) {
  a = par("par1").doubleValue();
  b = par("par2").doubleValue();
}

double UniformPauseTimeModel::computePauseTime() {
  return uniform(a, b);
}