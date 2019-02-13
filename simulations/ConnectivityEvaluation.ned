package slaw.simulations;

import slaw.engine.SlawEngine;
import slaw.walker.QPSWalker;
import slaw.app.ConnectivityObserver;

network SLAWConnectivityExperiment
{
    parameters:
        int numHosts;
        int ictSamples;
        int lltSamples;
        int x_num; //number of quadrants on x-axis
        int y_num; //number of quadrants on y-axis
        double side @unit(m); //The length of a side of a quadrant
        double minInterContactTime @unit(s);
        double minLifetime @unit(s);
        walker[*].qps.xQuadrant = x_num;
        walker[*].qps.sideLength = side;
        @display("bgb=2000,2000;bgg=1000,10,grey50");
    submodules:
        slaw: SlawEngine {
            @display("p=1800,1800");
        }
        walker[numHosts]: QPSWalker;
        observer: ConnectivityObserver {
            numOfNodes = numHosts;
            xQuadrant = x_num;
            yQuadrant = y_num;
            numOfICTSamples = ictSamples;
            numOfLLTSamples = lltSamples;
            radius = side / 2;
            minICT = minInterContactTime;
            minLLT = minLifetime;
            @display("p=1800,1900");
        }
}
