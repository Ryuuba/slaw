package slaw.simple_mobility;

import inet.mobility.base.MovingMobilityBase;

simple SlawMobility extends MovingMobilityBase
{
    parameters:
        @class(SlawMobility);
        string slawModuleName = default("slaw");
        @signal[flightLength](type=double);
        @statistic[flightLength](record=histogram,vector; unit=m);
        @signal[homeArrival](type=int);
        @display("i=block/cogwheel");
}
