package slaw.app;

simple ConnectivityObserver extends PositionObserver {
    parameters:
        @class(ConnectivityObserver);
        int numOfICTSamples;   //Intercontact time samples
        int numOfLLTSamples;    //link lifetime samples
        double radius @unit(m);
        double minICT @unit(s);
        double minLLT @unit(s);
        @signal[linkLifetime](type=double);
        @statistic[linkLifetime](record=stats,vector; unit=s);
        @signal[interContactTime](type=double);
        @statistic[interContactTime](record=stats,vector; unit=s;);
}