//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "PauseTime.h"

PauseTime::PauseTime(omnetpp::cRNG* rng) : rng(rng) {}

PauseTime::PauseTime(omnetpp::cRNG* rng, PauseTimeModel m, double a, double b, 
  double H) : rng(rng), model(m), param(a, b, H) {}

double PauseTime::pareto_bounded(double alpha, double L, double H) {
    double u = omnetpp::uniform(rng, 0, 1);
    double op1 = pow(H, alpha);
    double op2 = pow(L, alpha);
    //(-(UH^a - UL^a - H^a)/(H^a * L^a))^(-1/a) where U is random 0<=U<=1
    return pow(-(u*op1 - u*op2 - op1) / pow(H*L, alpha), -1.0/alpha);
}

void PauseTime::setPauseTimeModel(PauseTimeModel m) {
    model = m;
}

void PauseTime::setNormalModel(double mean, double stddev) {
    std::get<0>(param) = mean;
    std::get<1>(param) = stddev;
}

void PauseTime::setParetoBoundedModel(double alpha, double L, double H) {
    std::get<0>(param) = alpha;
    std::get<1>(param) = L;
    std::get<2>(param) = H;
}

void PauseTime::setUniformModel(double a, double b) {
    std::get<0>(param) = a;
    std::get<1>(param) = b;
}

void PauseTime::setConstantModel(double a) {
    std::get<0>(param) = a;
    std::get<1>(param) = 0.0;
}

double PauseTime::computePausetime() {
    double pausetime;
    switch(model) {
        case PauseTimeModel::CONSTANT: pausetime = std::get<0>(param);
            break;
        case PauseTimeModel::UNIFORM:
            pausetime = omnetpp::uniform(rng, std::get<0>(param), std::get<1>(param));
            break;
        case PauseTimeModel::NORMAL:
            pausetime = truncnormal(rng, std::get<0>(param), std::get<1>(param));
            break;
        default:
            pausetime = pareto_bounded(std::get<0>(param), std::get<1>(param),
               std::get<2>(param));
            break;
    }
    return pausetime;
}
