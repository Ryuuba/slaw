package slaw.simple_mobility;

import inet.mobility.base.MovingMobilityBase;

simple SlawMobility extends MovingMobilityBase
{
    parameters:
        @class(SlawMobility);
        bool classifyFlight = default(false);
        string slawModuleName = default("slaw");
        @signal[homeArrival](type=int);
        @signal[next_waypoint];
        @signal[trip_counter](type=long);
        @statistic[trip_counter](record=histogram);
        @signal[flight](type=double);
        @statistic[flight](record=histogram,vector; unit=m);
        @signal[intraFlightLength](type=double);
        @statistic[intraFlightLength](record=histogram,vector; unit=m);
        @signal[interFlightLength](type=double);
        @statistic[interFlightLength](record=histogram,vector; unit=m);
        @display("i=block/cogwheel");
}
