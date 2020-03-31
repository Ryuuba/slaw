#include "BParetoPauseTimeModel.h"

Define_Module(BParetoPauseTimeModel);

void BParetoPauseTimeModel::initialize(int stage) {
  if (stage == 0) {
    alpha = par("par1").doubleValue();
    x_min = par("par2").doubleValue();
    x_max = par("par3").doubleValue();
  }
}

double BParetoPauseTimeModel::computePauseTime() {
  Enter_Method_Silent();
  double u = uniform(0, 1);
  double op1 = pow(x_max, alpha);
  double op2 = pow(x_min, alpha);
  double observation = pow(-(u*op1 - u*op2 - op1) / pow(x_min*x_max, alpha), -1.0/alpha);
  return observation;
}