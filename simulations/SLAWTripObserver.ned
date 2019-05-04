package slaw.simulations;

import slaw.i_walker_model.IWalkerModel;
import slaw.walker.Walker;
import slaw.app.TripObserver;

network SLAWTripObserver
{
    parameters:
        walker.mobilityType = "SlawMobility";
        string slawModelType;
        slaw.numOfWalker = default(1);
        @display("bgb=10000,10000;bgg=1000,10,grey50");
    submodules:
        observer: TripObserver;
        slaw: <slawModelType> like IWalkerModel;
        walker: Walker;
}
