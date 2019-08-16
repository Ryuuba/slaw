package slaw.simple;

import inet.mobility.single.BonnMotionMobility;

simple SlawBonnMotion extends BonnMotionMobility
{
    parameters:
        string mapname;
        @class(SlawBonnMotion);
        @signal[flight](type=double);
        @statistic[flight](record=histogram,vector; unit=m);
        @display("i=block/cogwheel");
}