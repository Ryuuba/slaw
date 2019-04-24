package slaw.simulations;

import slaw.contract.IWalkerModel;
import slaw.walker.Walker;
import slaw.app.TripObserver;

network SLAWTripRecorder
{
    parameters:
        string output_file; //The file where the trip is written
        observer.filename = output_file;
        observer.trip_number = default(1);
        walker.mobilityType = "SlawMobility";
        string slawModelType;
        slaw.numOfWalker = 1;
        @display("bgb=2000,2000;bgg=1000,10,grey50");
    submodules:
        slaw: <slawModelType> like IWalkerModel;
        walker: Walker;
        observer: TripObserver;
}
