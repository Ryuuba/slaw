package slaw.simulations;

import slaw.i_walker_model.IWalkerModel;
import slaw.walker.Walker;
import slaw.app.ConnectivityObserver;

network SLAWConnectivityExperiment
{
    parameters:
        int numHosts;
        int ictSamples;
        int lltSamples;
        double width @unit(m); //length of the x-axis
        double height @unit(m); //length of the y-axis
        double coverageRadius @unit(m); //coverage radius
        double minInterContactTime @unit(s);
        double minLifetime @unit(s);
        string slawModelType;
        slaw.numOfWalker = numHosts;
        @display("bgb=2000,2000;bgg=100,2,grey50");
    submodules:
        slaw: <slawModelType> like IWalkerModel;
        walker[numHosts]: Walker {
            mobilityType = "SlawMobility";
        }
        observer: ConnectivityObserver {
            numOfNodes = numHosts;
            x_length = width;
            y_length = height;
            numOfICTSamples = ictSamples;
            numOfLLTSamples = lltSamples;
            radius = coverageRadius;
            minICT = minInterContactTime;
            minLLT = minLifetime;
            @display("p=480,400");
        }
}
