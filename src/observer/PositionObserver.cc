#include "PositionObserver.h"

Define_Module(PositionObserver);

omnetpp::simsignal_t 
  PositionObserver::position = registerSignal("mobilityStateChanged");

PositionObserver::PositionObserver()
{
  getSimulation()->getSystemModule()->subscribe(position, this);
}

PositionObserver::~PositionObserver() {
  getSimulation()->getSystemModule()->unsubscribe(position, this);
}

void PositionObserver::initialize(int stage) {
  if (stage == 0) {
    numOfNodes = par("numOfNodes");
    nodePosition.resize(numOfNodes);
    radius = par("radius").doubleValue();
    x_length = par("x_length").doubleValue();
    y_length = par("y_length").doubleValue();
    x_num = unsigned( ceil(x_length / radius) );
    y_num = unsigned( ceil(y_length / radius) );
    nodeMap.resize(x_num * y_num);
  }
}

void PositionObserver::handleMessage(omnetpp::cMessage* msg) {
  error("Connectivity Observer: This module does not receive messages");
}

void PositionObserver::receiveSignal(omnetpp::cComponent* src, omnetpp::simsignal_t id, omnetpp::cObject* value, omnetpp::cObject* details) {
  nodeId = dynamic_cast<omnetpp::cModule*>(src)->getParentModule()->getIndex();
  auto state = dynamic_cast<inet::MovingMobilityBase*>(value);
  inet::Coord currentPosition(state->getCurrentPosition());
  unsigned currentSquare = computeSquare(currentPosition);
  unsigned lastSquare = computeSquare(nodePosition[nodeId]);
  nodePosition[nodeId] = currentPosition;
  if (currentSquare != lastSquare)
  {
    auto it = std::find(nodeMap[lastSquare].begin(), nodeMap[lastSquare].end(), nodeId);   
    if (it != nodeMap[lastSquare].end())
      nodeMap[lastSquare].erase(it);
    nodeMap[currentSquare].push_back(nodeId);
  }
  EV_INFO << "Node " << nodeId << " changes its position to square "
          << currentSquare << " in coordinate <" << nodePosition[nodeId].x 
          << ", " << nodePosition[nodeId].y<< ">\n";
}

std::list<unsigned> PositionObserver::computeNeighboringSquares(unsigned s)
{
  std::list<unsigned> squareList;
  bool left(s%x_num == 0), right(s%x_num == x_num-1);
  bool top(s < x_num), bottom(s > x_num*(y_num-1) - 1);
  squareList.push_back(s); //Always search in square s
  if (!(top || left)) squareList.push_back(s-x_num-1);
  if (!top) squareList.push_back(s-x_num);
  if (!(top || right)) squareList.push_back(s-x_num+1);
  if (!right) squareList.push_back(s+1);
  if (!(bottom || right)) squareList.push_back(s+x_num+1);
  if (!bottom) squareList.push_back(s+x_num);
  if (!(bottom || left)) squareList.push_back(s+x_num-1);
  if (!left) squareList.push_back(s-1);
  return squareList;
}

unsigned PositionObserver::computeSquare(const inet::Coord& c)
{
  return x_num * unsigned(floor(c.y / radius)) + unsigned(floor(c.x / radius));
}