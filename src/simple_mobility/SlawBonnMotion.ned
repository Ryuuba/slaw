package slaw.simple_mobility;

import inet.mobility.single.BonnMotionMobility;

simple SlawBonnMotion extends BonnMotionMobility
{
    parameters:
        string mapname;
        @class(SlawBonnMotion);
        @signal[flightLength](type=double);
        @statistic[flightLength](record=histogram,vector; unit=m);
        @display("i=block/cogwheel");
}