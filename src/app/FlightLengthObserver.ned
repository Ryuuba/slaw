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

package slaw.app;
import slaw.contract.IObserver;

simple FlightLengthObserver like IObserver
{
    @display("i=block/cogwheel");
    int observations;
    bool classifyFlight = default(false);
    @signal[flight_stat](type=double);
    @statistic[flight_stat](record=histogram,vector; unit=m);
    @signal[intraFlightLength_stat](type=double);
    @statistic[intraFlightLength_stat](record=histogram,vector; unit=m);
    @signal[interFlightLength_stat](type=double);
    @statistic[interFlightLength_stat](record=histogram,vector; unit=m);
    @signal[remoteRandomArea_stat](type=bool);
    @statistic[remoteRandomArea_stat](record=vector);
}
