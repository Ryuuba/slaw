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
import slaw.observer.FlightLengthObserver;
import slaw.tripmanager.TripManager;


network SLAWFlightLengthExperiment
{
    parameters:
        int numHosts;
        int observations;
        string pauseTimeModelType;
        string speedModelType;
        string walkerModelType;
        string observerType = "FlightLengthObserver";
        bool classifyFlight;
        observer.classifyFlight = classifyFlight;
        tripmanager.walkerModelType = walkerModelType;
        tripmanager.walkermodel.numOfWalker = numHosts;
        tripmanager.walkermodel.classifyFlight = classifyFlight;
        tripmanager.pauseTimeModelType = pauseTimeModelType;
        tripmanager.speedModelType = speedModelType;
        @display("bgb=2000,2000;bgg=2000,10,grey50");
    submodules:
        tripmanager: TripManager {
            @display("p=200,200");
        }
        observer: <observerType> like IObserver {
            observations = observations;
            @display("p=200,550");
        }
        walker[numHosts]: Walker {
            mobilityType = "SlawMobility";
            @display("p=1000,500");
        }
}
