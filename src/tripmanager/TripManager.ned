//
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

package slaw.tripmanager;
import slaw.contract.IPauseTimeModel;
import slaw.contract.ISpeedModel;
import slaw.contract.IWalkerModel;
import slaw.map.SelfSimilarWaypointMap;
import slaw.latp.LATPAlgorithm;

module TripManager
{
    parameters:
        @networkNode;
        @display("i=block/cogwheel");
        string walkerModelType;
        string pauseTimeModelType;
        string speedModelType;
    submodules:
        pausetimemodule: <pauseTimeModelType> like IPauseTimeModel {
            @display("p=70,50;i=block/pause");
        }
        speedmodule: <speedModelType> like ISpeedModel {
            @display("p=70,125;i=block/speedometer");
        }
        walkermodel: <walkerModelType> like IWalkerModel {
            @display("p=70,200;i=block/user");
        }
        mapmodule: SelfSimilarWaypointMap {
            @display("p=200,50");
        }
        latpmodule: LATPAlgorithm {
            @display("p=200,125");
        }
}
