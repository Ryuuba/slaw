/**
 * Self-similar least-action walk mobility model. See NED file for more info.
 *
 * @author Adán G. Medrano-Chávez,
 * Universidad Autónoma Metropolitana - Cuajimalpa, agmedrano@correo.cua.uam.mx
 * @author Georg Lutz (georglutz AT gmx DOT de), Institut fuer Telematik,
 * Universitaet Karlsruhe, http://www.tm.uka.de, 2004-2005 
 * (generalized, ported to RandomWPMobility)
 * @author Andras Varga (generalized, ported to LineSegmentsMobilityBase)
 */

#ifndef SLAW_MOBILITY_H
#define SLAW_MOBILITY_H
#include <cstdint>
#include <omnetpp.h>
#include <omnetpp/csimplemodule.h>

#include "../contract/IWalkerModel.h"
#include "../signals/WaypointSignal.h"
#include "LineSegmentsMobilityBase.h"

class SlawMobility : public inet::LineSegmentsMobilityBase
{
protected:
  /** @brief Signal conveying the next waypoint*/
  WaypointSignal nwp;
  /** @brief Walker ID */
  unsigned walkerID;
  /** @brief The distance to reach the next destination */
  double distance;
  /** @brief Signal carrying the next waypoint */
  static omnetpp::simsignal_t next_waypoint;
  /** @brief Signal informing node flight lengths */
  static omnetpp::simsignal_t flight, intraFlightLength, interFlightLength;
  /** @brief Signal carrying the next area to be visited is randomly drawn */
  static omnetpp::simsignal_t remote_random_area;
  /** @brief Signal carrying the size of new trips */
  static omnetpp::simsignal_t trip_size;
  /** Boolean value used to switch between these states {pause, motion} */
  bool nextMoveIsWait, classifyFlight, isNewTrip;
  /** Pointer to the SLAW module, it is used to access the selfsimilar
   *  waypoint map and it computes the next destination using LATP. */
  IWalkerModel* slaw;
  /** @brief Vector storing the sets of confined areas. */
  AreaSet C_k;
  /** @brief List storing the set of waypoints integrating a trip. */
  WaypointList unvisitedWaypointList;
protected:
  /** @brief Overridden from MobilityBase.*/
  virtual void setInitialPosition() override;
  /** Number of states used to initialize every module derived from
   *  MobilityBase. This module is initialized in the state 0. It is very
   *  important place it below the slaw model in he ned file in order
   *  to avoid the error 139: segmentation fault. */
  virtual int numInitStages() const { return 6; }
  /** @brief Initializes mobility model parameters.*/
  virtual void initialize(int stage);
  /** @brief Overridden from LineSegmentsMobilityBase.*/
  virtual void setTargetPosition() override;
  /** @brief Overridden from LineSegmentsMobilityBase.*/
  virtual void move() override;
  /** @brief Emits all signals */
  virtual void emitSignals();
public:
  /** @brief Default constructor. It initializes the motion status and the
   *  value of the slaw pointer. */
  SlawMobility();
};

#endif
