package slaw.simple_mobility;

import inet.mobility.base.MovingMobilityBase;

simple SlawMobility extends MovingMobilityBase
{
    parameters:
        @class(SlawMobility);
        string slawModuleName = default("slaw");
        @signal[intraFlightLength](type=double);
        @statistic[intraFlightLength](record=histogram,vector; unit=m);
        @signal[interFlightLength](type=double);
        @statistic[interFlightLength](record=histogram,vector; unit=m);
        @signal[homeArrival](type=int);
        @display("i=block/cogwheel");
}
