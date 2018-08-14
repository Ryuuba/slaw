#include "PositionObserver.h"

Define_Module(PositionObserver);

omnetpp::simsignal_t 
  PositionObserver::quadrant = registerSignal("quadrant");

PositionObserver::PositionObserver()
  : numOfNodes(0), x_num(0), y_num(0), nodeId(0) {
  getSimulation()->getSystemModule()->subscribe(quadrant, this);
}

PositionObserver::~PositionObserver() {
  getSimulation()->getSystemModule()->unsubscribe(quadrant, this);
}

void PositionObserver::initialize() {
  numOfNodes = par("numOfNodes");
  nodePosition.resize(numOfNodes);
  x_num = par("xQuadrant");
  y_num = par("yQuadrant");
  nodeMap.resize(x_num * y_num);
}

void PositionObserver::handleMessage(omnetpp::cMessage* msg) {
  error("Connectivity Observer: This module does not receive messages");
}

void PositionObserver::receiveSignal(omnetpp::cComponent* src, omnetpp::simsignal_t id, omnetpp::cObject* value, omnetpp::cObject* details) {
  nodeId = dynamic_cast<omnetpp::cModule*>(src)->getParentModule()->getIndex();
  currentQuadrant = *(dynamic_cast<QuadrantCoordinate*>(value));
  lastQuadrant = nodePosition[nodeId];
  //If current quadrant is different from the quadrant stored in nodePosition
  //then it updates nodeMap moving the node to its actual quadrant, then updates
  //its position
  if ((currentQuadrant.q == lastQuadrant.q))           
    nodePosition[nodeId] = currentQuadrant;
  else {        
    auto it = std::find(nodeMap[lastQuadrant.q].begin(), nodeMap[lastQuadrant.q].end(), nodeId);   
    if (it != nodeMap[lastQuadrant.q].end())
      nodeMap[lastQuadrant.q].erase(it);
    nodeMap[currentQuadrant.q].push_back(nodeId);
    nodePosition[nodeId] = currentQuadrant;
  }
  EV_INFO << "Node " << nodeId << " changes its position <"
          << nodePosition[nodeId].q << ", " << nodePosition[nodeId].subq
          << "; " << nodePosition[nodeId].x << ", " << nodePosition[nodeId].y
          << ">\n";
}

std::list<unsigned> PositionObserver::computeNeighboringQuadrants(unsigned q, unsigned subq) {
  std::list<unsigned> qList;
  bool left(q%x_num == 0), right(q%x_num == x_num-1);
  bool up(q < x_num), down(q > x_num*(y_num-1) - 1);
  qList.push_back(q); //Always search in q
  //TODO: optimize this code
  if (subq == 0) {
    if (!left)
      qList.push_back(q-1);
    if (!left && !up)
      qList.push_back(q-x_num-1);
    if (!up)
      qList.push_back(q-x_num);
  }
  else if (subq == 1) {
    if (!up)
      qList.push_back(q-x_num);
    if (!up && !right)
      qList.push_back(q-x_num+1);
    if (!right)
      qList.push_back(q+1);
  }
  else if (subq == 2) {
    if (!left)
      qList.push_back(q-1);
    if (!left && !down)
      qList.push_back(q+x_num-1);
    if (!down)
      qList.push_back(q+x_num);
  }
  else {
    if (!right)
      qList.push_back(q+1);
    if (!right && !down)
      qList.push_back(q+x_num+1);
    if (!down)
      qList.push_back(q+x_num);
  }
  return qList;
}