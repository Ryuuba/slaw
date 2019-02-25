package slaw.simulations;

import slaw.engine.SlawEngine;
import slaw.walker.Walker;
import slaw.app.TripObserver;

network SLAWTripRecorder
{
    parameters:
        string output_file; //The file where the trip is written
        observer.filename = output_file;
        observer.trip_number = default(1);
        string mobility = default("SlawMobility");
        @display("bgb=2000,2000;bgg=1000,10,grey50");
    submodules:
        slaw: SlawEngine {
            @display("p=1800,1800");
        }
        walker: Walker {
            mobilityType = mobility;
        }
        observer: TripObserver;
}
