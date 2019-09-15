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

package slaw.walkermodel;

import slaw.contract.ISlawTripManager;
import slaw.contract.IPauseTime;
import slaw.contract.ISpeed;

simple SlawTransNetw like ISlawTripManager {
  parameters:
    @display("i=block/cogwheel");
    string walkerModel = "SlawTransNetw";
    int numOfWalker;
    string mapName;
    string clusterList;
    double clusteringRadius @unit(m);
    //CONSTANT = 1, UNIFORM = 2, NORMAL = 3, PARETO_BOUNDED = 4
    string pauseTimeModule = "pausetimemodule";
    //CONSTANT = 1, NORMAL = 3, RHEE = 4
    string speedModule = "speedmodule";
    double speedA @unit(mps);
    double speedB @unit(mps);
    double planningDegree;
    double hurstParameter;  
}
