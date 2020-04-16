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
#if !defined(LATP_ALGORITHM_H)
#define LATP_ALGORITHM_H

#include "../common/SlawDefs.h"

class LATPAlgorithm : public omnetpp::cSimpleModule {
private:
  double a;           //planning degree
public:
  LATPAlgorithm()
  : a {0.0}
  { }
  virtual ~LATPAlgorithm() { }
  virtual inet::Coord operator()(inet::Coord&, WaypointList&);
  virtual int numInitStages() const override {return 6;}
  virtual void initialize(int) override;
  virtual void handleMessage(omnetpp::cMessage*) {
    error("LATPAlgorithm: This module does not receive any message\n");
  }

};


#endif /* LATP_ALGORITHM_H */
