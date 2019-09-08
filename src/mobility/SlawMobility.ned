package slaw.mobility;

import inet.mobility.base.MovingMobilityBase;

simple SlawMobility extends MovingMobilityBase
{
    parameters:
        string slawModuleName = default("slawtripmanager");
        bool classifyFlight = default(false);
        @class(SlawMobility);
        @signal[nextWaypoint](type=cObject);
        @signal[tripSize](type=long);
        @signal[flight](type=double);
        @signal[intraFlightLength](type=double);
        @signal[interFlightLength](type=double);
        @signal[remoteRandomArea](type=bool);
        @display("i=block/cogwheel");
}
