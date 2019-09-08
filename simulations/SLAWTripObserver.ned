package slaw.simulations;

import slaw.contract.ISlawTripManager;
import slaw.walker.Walker;
import slaw.app.TripObserver;

network SLAWTripObserver
{
    parameters:
        walker.mobilityType = "SlawMobility";
        string slawModelType;
        slawtripmanager.numOfWalker = default(1);
        @display("bgb=10000,10000;bgg=1000,10,grey50");
    submodules:
        observer: TripObserver;
        slawtripmanager: <slawModelType> like ISlawTripManager;
        walker: Walker;
}
