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

package slaw.simulations;

import slaw.engine.SlawEngine;
import slaw.simple_mobility.SlawBonnMotion;
import slaw.walker.Walker;
import slaw.app.FlightLengthObserver;


network SLAWFlightLengthExperiment
{
    parameters:
        int numHosts;
        int numSamples;
        string mobility = default("SlawMobility");
        @display("bgb=2000,2000;bgg=2000,10,grey50");
    submodules:
        slaw: SlawEngine if mobility == "SlawMobility" {
            @display("p=62.247997,925.688");
        }
        walker[numHosts]: Walker {
            mobilityType = mobility;
        }
        observer: FlightLengthObserver {
            numOfSamples = numSamples;
            @display("p=62.247997,795.16797");
        }
}
