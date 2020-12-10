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

package slaw.simulations;

import slaw.contract.IObserver;
import slaw.walker.Walker;
import slaw.tripmanager.TripManager;

network SLAWConnectivityEvaluation
{
    parameters:
        int numHosts;
        int observations;
        double width @unit(m); //length of the x-axis
        double height @unit(m); //length of the y-axis
        double coverageRadius @unit(m); //coverage radius
        int minLifetime @unit(s);
        string walkerModelType;
        string pauseTimeModelType;
        string speedModelType;
        string filename;
        tripmanager.walkerModelType = walkerModelType;
        tripmanager.walkermodel.numOfWalker = numHosts;
        tripmanager.pauseTimeModelType = pauseTimeModelType;
        tripmanager.speedModelType = speedModelType;
        observer.filename = filename;
        observer.numOfNodes = numHosts;
        observer.observations = observations;
        observer.xLength = width;
        observer.yLength = height;
        observer.radius = coverageRadius;
        observer.minLLT = minLifetime;
        @display("bgb=1000,1000;bgg=100,2,grey50");
    submodules:
        tripmanager: TripManager {
            @display("p=48.191998,246.984");
        }
        walker[numHosts]: Walker {
            mobilityType = "SlawMobility";
            @display("p=883.51996,887.53595");
        }
        observer: <"ConnectivityObserver"> like IObserver {
            @display("p=48.191998,100.399994");
        }
}