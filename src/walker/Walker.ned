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

package slaw.walker;

import slaw.simple_mobility.SlawMobility;
//
// A host for demonstrating mobility models only -- it contains
// no protocol layers at all.
//
module Walker
{
    parameters:
        @networkNode;
        @display("i=old/ball,white,50;is=s");
        @statistic[coordX](title="coord X"; source="xCoord(mobilityPos(mobilityStateChanged))"; record=vector; interpolationmode=linear);
        @statistic[coordY](title="coord Y"; source="yCoord(mobilityPos(mobilityStateChanged))"; record=vector; interpolationmode=linear);
        //@statistic[coordZ](title="coord Z";source="zCoord(mobilityPos(mobilityStateChanged))";record=vector; interpolationmode=linear);
    submodules:
        mobility: SlawMobility;
}
