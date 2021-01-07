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

package slaw.observer;
import slaw.contract.IObserver;

simple ChurnObserver like IObserver{
    parameters:
        int observations;
        string mapModule = default("mapmodule");
        @class(ChurnObserver);
        @signal[membership](type=unsigned long);
        @statistic[membership](record=histogram,vector);
        @signal[arrival](type=unsigned long);
        @statistic[arrival](record=histogram,vector);
        @signal[departure](type=unsigned long);
        @statistic[departure](record=histogram,vector);
}