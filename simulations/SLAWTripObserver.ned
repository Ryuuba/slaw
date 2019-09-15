package slaw.simulations;

import slaw.contract.ISlawTripManager;
import slaw.contract.IPauseTimeModel;
import slaw.contract.ISpeedModel;
import slaw.contract.IObserver;
import slaw.walker.Walker;
import slaw.app.TripObserver;

network SLAWTripObserver
{
    parameters:
        walker.mobilityType = "SlawMobility";
        double par1;
        double par2;
        double par3;
        string slawModelType;
        string pauseTimeModelType;
        string speedModelType;
        string observerType = "TripObserver";
        slawtripmanager.numOfWalker = default(1);
        pausetimemodule.par1 = par1;
        pausetimemodule.par2 = par2;
        pausetimemodule.par3 = par3;
        @display("bgb=2000,2000;bgg=100,1,grey50");
    submodules:
        pausetimemodule: <pauseTimeModelType> like IPauseTimeModel;
        speedmodule: <speedModelType> like ISpeedModel;
        slawtripmanager: <slawModelType> like ISlawTripManager;
        observer: <observerType> like IObserver;
        walker: Walker;
}