package slaw.simulations;

import slaw.contract.ISlawTripManager;
import slaw.contract.IPauseTimeModel;
import slaw.walker.Walker;
import slaw.app.TripObserver;

network SLAWTripObserver
{
    parameters:
        walker.mobilityType = "SlawMobility";
        double par1;
        double par2 @unit(s);
        double par3 @unit(s);
        string slawModelType;
        string pauseTimeModelType;
        slawtripmanager.numOfWalker = default(1);
        pausetimemodule.par1 = 10;
        pausetimemodule.par2 = 2;
        pausetimemodule.par3 = par3;
        @display("bgb=2000,2000;bgg=100,1,grey50");
    submodules:
        pausetimemodule: <pauseTimeModelType> like IPauseTimeModel;
        slawtripmanager: <slawModelType> like ISlawTripManager;
        observer: TripObserver;
        walker: Walker;
}

//BParetoPauseTimeModel
//ConstantPausTimeModel
//NormalPauseTimeModel
//UniformPauseTimeModel