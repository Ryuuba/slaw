package slaw.simulations;

import slaw.contract.ISlawTripManager;
import slaw.contract.IObserver;
import slaw.contract.IPauseTimeModel;
import slaw.contract.ISpeedModel;
import slaw.walker.Walker;
import slaw.app.ConnectivityObserver;

network SLAWConnectivityExperiment
{
    parameters:
        int numHosts;
        int observations;
        double width @unit(m); //length of the x-axis
        double height @unit(m); //length of the y-axis
        double coverageRadius @unit(m); //coverage radius
        double minInterContactTime @unit(s);
        double minLifetime @unit(s);
        string slawModelType;
        string pauseTimeModelType;
        string speedModelType;
        string observerType = "InterContactTimeObserver";
        slawtripmanager.numOfWalker = numHosts;
        @display("bgb=2000,2000;bgg=100,2,grey50");
    submodules:
        slawtripmanager: <slawModelType> like ISlawTripManager;
        walker[numHosts]: Walker {
            mobilityType = "SlawMobility";
        }
        observer: <observerType> like IObserver {
            numOfNodes = numHosts;
            observations = observations;
            x_length = width;
            y_length = height;
            radius = coverageRadius;
            minICT = minInterContactTime;
            minLLT = minLifetime;
            @display("p=480,400");
        }
        pausetimemodule: <pauseTimeModelType> like IPauseTimeModel;
        speedmodule: <speedModelType> like ISpeedModel;
}
