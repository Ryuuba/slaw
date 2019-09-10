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

#ifndef I_OBSERVER_H
#define I_OBSERVER_H

#include <omnetpp.h>

class IObserver :
  public omnetpp::cSimpleModule,
  public omnetpp::cListener
{
protected:
  /** @brief The number of observations to be gathered to finish simulations */
  int sample_size;
  /** @brief The total of observations to be captured */
  unsigned counter;
  /** @brief  The name of the observer module */
  const char* observer_type;
public:
  IObserver() : counter(0), sample_size(0) {};
  virtual void initialize(int stage) override;
  virtual int numInitStages() const = 0;
};

Register_Abstract_Class(IObserver);

#endif /* I_OBSERVER_H */