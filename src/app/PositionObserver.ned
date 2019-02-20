package slaw.app;

simple PositionObserver {
    @display("i=block/cogwheel");
    int numOfNodes;
    double radius @unit(m);
    double x_length @unit(m);
    double y_length @unit(m);
}