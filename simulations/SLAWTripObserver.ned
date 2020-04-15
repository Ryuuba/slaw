package slaw.simulations;

import slaw.tripmanager.TripManager;
import slaw.contract.IObserver;
import slaw.walker.Walker;
import slaw.observer.TripObserver;

network SLAWTripObserver
{
    parameters:
        walker.mobilityType = "SlawMobility";
        double par1;
        double par2;
        double par3;
        string walkerModelType;
        string pauseTimeModelType;
        string speedModelType;
        string observerType = "TripObserver";
        pausetimemodule.par1 = par1;
        pausetimemodule.par2 = par2;
        pausetimemodule.par3 = par3;
        tripmanager.walkerModelType = walkerModelType;
        tripmanager.walkermodel.numOfWalker = default(1);
        tripmanager.pauseTimeModelType = pauseTimeModelType;
        tripmanager.speedModelType = speedModelType;
        @display("bgb=2000,2000;bgg=100,1,grey50");
    submodules:
        tripmanager: TripManager {
            @display("p=200,200");
        }
        observer: <observerType> like IObserver {
            @display("p=200,550");
        }
        walker: Walker {
            @display("p=1000,500");
        }
}
