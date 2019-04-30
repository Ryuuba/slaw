package slaw.app;

simple FlightLengthObserver
{
    int numOfSamples;
    bool classifyFlight = default(false);
    @display("i=misc/sun;is=n");
    @signal[trip_counter_stat](type=long);
    @statistic[trip_counter_stat](record=histogram);
    @signal[flight_stat](type=double);
    @statistic[flight_stat](record=histogram,vector; unit=m);
    @signal[intraFlightLength_stat](type=double);
    @statistic[intraFlightLength_stat](record=histogram,vector; unit=m);
    @signal[interFlightLength_stat](type=double);
    @statistic[interFlightLength_stat](record=histogram,vector; unit=m);
}
