package slaw.simple_mobility;

import inet.mobility.base.MovingMobilityBase;

simple SlawMobility extends MovingMobilityBase
{
    parameters:
        string slawModuleName = default("slaw");
        bool classifyFlight = default(false);
        @class(SlawMobility);
        @signal[homeArrival](type=int);
        @signal[next_waypoint];
        @signal[trip_counter](type=long);
        @statistic[trip_counter](record=histogram);
        @signal[flight](type=double);
        @signal[flightKind](type=bool); //0:intra, 1:inter
        @display("i=block/cogwheel");
}
