package slaw.simulations;

import slaw.contract.IObserver;
import slaw.walker.Walker;
import slaw.app.ConnectivityObserver;
import slaw.tripmanager.TripManager;

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
        string walkerModelType;
        string pauseTimeModelType;
        string speedModelType;
        tripmanager.walkerModelType = walkerModelType;
        tripmanager.walkermodel.numOfWalker = numHosts;
        tripmanager.pauseTimeModelType = pauseTimeModelType;
        tripmanager.speedModelType = speedModelType;
        string observerType = "InterContactTimeObserver";
        @display("bgb=2000,2000;bgg=100,2,grey50");
    submodules:
        tripmanager: TripManager {
            @display("p=175,450");
        }
        walker[numHosts]: Walker {
            mobilityType = "SlawMobility";
            @display("p=1550,350");
        }
        observer: <observerType> like IObserver {
            numOfNodes = numHosts;
            observations = observations;
            x_length = width;
            y_length = height;
            radius = coverageRadius;
            minICT = minInterContactTime;
            minLLT = minLifetime;
            @display("p=175,175");
        }
}
