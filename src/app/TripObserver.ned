package slaw.app;

simple TripObserver {
    @display("i=block/cogwheel");
    int sampleSize;
    @signal[nextWaypointX](type=double);
    @statistic[nextWaypointX](record=vector);
    @signal[nextWaypointY](type=double);
    @statistic[nextWaypointY](record=vector);
    @signal[tripSize_stat](type=long);
    @statistic[tripSize_stat](record=histogram,vector);
}