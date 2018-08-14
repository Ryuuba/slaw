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
import slaw.walker.QPSWalker;
import slaw.app.ConnectivityObserver;

network SLAWConnectivityExperiment
{
    parameters:
        int numHosts;
        int ictSamples;
        int lltSamples;
        int x_num; //number of quadrants on x-axis
        int y_num; //number of quadrants on y-axis
        double side @unit(m); //The length of a side of a quadrant
        double minInterContactTime @unit(s);
        double minLifetime @unit(s);
        walker[*].qps.xQuadrant = x_num;
        walker[*].qps.sideLength = side;
        @display("bgb=1000,1000;bgg=1000,10,grey50");
    submodules:
        slaw: SlawEngine {
            @display("p=62.247997,925.688");
        }
        walker[numHosts]: QPSWalker {
            @display("p=624.488,481.91998");
        }
        observer: ConnectivityObserver {
            numOfNodes = numHosts;
            xQuadrant = x_num;
            yQuadrant = y_num;
            numOfICTSamples = ictSamples;
            numOfLLTSamples = lltSamples;
            radius = side / 2;
            minICT = minInterContactTime;
            minLLT = minLifetime;
            @display("p=62.247997,795.16797");
        }
}
