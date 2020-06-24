#include "NormalPauseTimeModel.h"

Define_Module(NormalPauseTimeModel);

void NormalPauseTimeModel::initialize(int atage) {
  mean = par("par1").doubleValue();
  std_dev = par("par2").doubleValue();
}

double NormalPauseTimeModel::computePauseTime() {
  Enter_Method_Silent();
  return truncnormal(mean, std_dev);
}