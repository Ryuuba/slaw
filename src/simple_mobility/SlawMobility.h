/**
 * Self-similar least-action walk mobility model. See NED file for more info.
 *
 * @author Adán G. Medrano-Chávez,
 * Universidad Autónoma Metropolitana - Iztapalapa, adme@xanum.uam.mx
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

#include "../trip_manager/SlawTripManager.h"
#include "LineSegmentsMobilityBase.h"

class SlawMobility : public inet::LineSegmentsMobilityBase
{
protected:
  /** @brief Walker ID */
  unsigned walkerID;
  /** @brief The distance to reach the next destination */
  double distance;
  /** @brief the number of trips completed by a walker */
  long counter;
  /** @brief Signal carrying the next waypoint */
  static omnetpp::simsignal_t next_waypoint;
  /** @brief Signal informing node flight lengths */
  static omnetpp::simsignal_t flight, intraFlightLength, interFlightLength;
  /** @brief Signal carrying the number of trips a walker has completed */
  static omnetpp::simsignal_t trip_counter;
  /** Boolean value used to switch between these states {pause, motion} */
  bool nextMoveIsWait, classifyFlight, isNewTrip;
  /** Pointer to the SLAW module, it is used to access the selfsimilar
   *  waypoint map and it computes the next destination using LATP. */
  SlawTripManager* slaw;
  /** Name of the module executing Slaw. It is defined in a ned file. Its
   *  default value is merely "slaw". */
  std::string slawModuleName;
  /** @brief Vector storing the sets of confined areas. */
  AreaSet C_k;
  /** @brief List storing the set of waypoints integrating a trip. */
  WaypointList unvisitedWaypointList;
protected:
  /** Number of states used to initialize every module derived from
   *  MobilityBase. This module is initialized in the state 0. It is very
   *  important place it below the slaw model in he ned file in order
   *  to avoid the error 139: segmentation fault. */
  virtual int numInitStages() const { return 3; }
  /** @brief Initializes mobility model parameters.*/
  virtual void initialize(int stage);
  /** @brief Overridden from MobilityBase.*/
  virtual void setInitialPosition() override;
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
