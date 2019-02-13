package slaw.simulations;

import slaw.engine.SlawEngine;
import slaw.walker.QPSWalker;
import slaw.app.TripObserver;

network SLAWTripRecorder
{
    parameters:
        string output_file; //The file where the trip is written
        int x_num; //number of quadrants on x-axis
        int y_num; //number of quadrants on y-axis
        double side @unit(m); //The length of a side of a quadrant
        walker.qps.xQuadrant = x_num;
        walker.qps.yQuadrant = y_num;
        walker.qps.sideLength = side;
        observer.filename = output_file;
        observer.trip_number = default(1);
        @display("bgb=2000,2000;bgg=1000,10,grey50");
    submodules:
        slaw: SlawEngine {
            @display("p=1800,1800");
        }
        walker: QPSWalker;
        observer: TripObserver;
}
