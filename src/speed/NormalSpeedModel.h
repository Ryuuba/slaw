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
#if !defined(NORMAL_SPEED_MODEL_H)
#define NORMAL_SPEED_MODEL_H

#include "../contract/ISpeedModel.h"

class NormalSpeedModel : public ISpeedModel
{
protected:
  double mean, std_dev;
public:
  virtual double computeSpeed(double par = 0.0) override;
  virtual void initialize(int stage) override;
  virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; } 
};

#endif /* NORMAL_SPEED_MODEL_H */