#include "QuadrantPositionSystem.h"

Define_Module(QuadrantPositionSystem);

// This signal is emitted by all modules inheritating from the IMobility
// interface
omnetpp::simsignal_t 
  QuadrantPositionSystem::position = registerSignal("mobilityStateChanged");
omnetpp::simsignal_t 
  QuadrantPositionSystem::quadrant = registerSignal("quadrant");

QuadrantPositionSystem::QuadrantPositionSystem() { };

QuadrantPositionSystem::~QuadrantPositionSystem() {
  getParentModule()->unsubscribe(position, this);
}

unsigned QuadrantPositionSystem::computeQuadrant() {
  coordinate.q = unsigned(coordinate.x/sideLength) + 
    unsigned(coordinate.y/sideLength) * unsigned(x_num);
  return coordinate.q;
};

unsigned QuadrantPositionSystem::computeSubquadrant() {
	double sub_x = coordinate.x - (coordinate.q%x_num) * sideLength;
	double sub_y = coordinate.y - (coordinate.q - (coordinate.q%x_num))/x_num * sideLength;
  coordinate.subq = unsigned(2*sub_x/sideLength) + 
    2 * unsigned(2*sub_y/sideLength);
  return coordinate.subq;
};

void QuadrantPositionSystem::handleMessage(omnetpp::cMessage* msg) {
  error("QPS: This module does not receive messages!");
}

void QuadrantPositionSystem::initialize() {
  x_num = par("xQuadrant");
  sideLength = par("sideLength");
  getParentModule()->subscribe(position, this);
  WATCH(coordinate.q);
  WATCH(coordinate.subq);
  WATCH(coordinate.x);
  WATCH(coordinate.y);
}

void QuadrantPositionSystem::receiveSignal(omnetpp::cComponent* src, 
  omnetpp::simsignal_t id, omnetpp::cObject* obj, omnetpp::cObject *details) {
  inet::MovingMobilityBase* mobilityObj = dynamic_cast<inet::MovingMobilityBase*>(obj);  
  coordinate.x = mobilityObj->getCurrentPosition().x; 
  coordinate.y = mobilityObj->getCurrentPosition().y; 
  unsigned nodeId = dynamic_cast<cModule*>(src)->getParentModule()->getIndex();
  computeQuadrant();     
  computeSubquadrant();   
  omnetpp::cObject* quadrantObj = dynamic_cast<omnetpp::cObject*>(&coordinate);
  emit(quadrant, quadrantObj);
}