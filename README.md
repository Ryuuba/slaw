# Slaw++
A SLAW mobility simulator based on the OMNeT++ and INET frameworks

## Description
Slaw++ is a simulator that implements the _Self-similar Least-Action Walk_ model (SLAW). SLAW is a realistic human mobility model that was designed by Lee, Hong, Kim, Rhee, and Chong (2012). According to their authors, SLAW represents five features of human motion:
  1. Heavy-tail flights and pause-times
  2. Heterogeneously bounded mobility areas
  3. Truncated power-law intercontact times (ICTs)
  4. Self-similar waypoints
  5. Least-action trip planning

Different from other trace-based approaches, Slaw++ is designed keeping in mind performance since it computes next destinations on-demand_. This means you do not need a large amount of memory to store the data from mobility trace files. This is due to the fact that Slaw++ computes both waypoint lists and next waypoints when it is need it, i.e., when initializing the simulation, when a trip is over, as well as when a walker reaches a waypoint. Therefore, you save several clock cycles since accessing main memory to read the next destination of a walker is avoided. In addition, this approach is especially useful when you need to run long lasting experiments, a large number of them, or dense scenarios with a large number of nodes. By extension, this implies you do not have to worry about running out of samples before the experiments are finished.

Another important features of Slaw++ is that it implements the two available _Individual Walker Models_ (IWMs) of SLAW: the one used in the original trace generator implemented in Matlab and the one presented in Lee et al. (2012). We denote these models as m-wm and t-wm, respectively. The m-wm is validated by means of a two-sample K-S test so that the flights it produces are statistically equivalent to the flights produced by the original SLAW trace generator. The t-wm is not implemented in other simulation tools, so Slaw++ is possibly the first simulator implementing such walker model. Due to the fact that IWMs are mainly based on intuition, the Slaw++ simulator is designed to implement other IWMs when future analysis on human motion are available.

## Installation
In order to install Slaw++, you must have a functional installation of both frameworks: [OMNeT++ 5 or greater](https://omnetpp.org/download/) and [INET 4 or greater](https://inet.omnetpp.org/Download.html).

Once you have installed the above mentioned frameworks, follow these steps:
  1. Clone [Slaw++](https://github.com/Ryuuba/slaw)
  2. Modify the INETDIR variable in the Makefile so that the value matches with the directory where you compiled the INET framework
  3. Execute the make makefiles command
  ```bash
  $ make makefiles
  ```
  4. Execute the make command
  ```bash
  $ make
  ```
  5. Done!

## Architecture
The Slaw++ simulator is based on the following architecture. A general explanation about all classes is provided as follows.![architecture](modules_alternative.svg "Slaw++ architecture"). 

The _SlawTripManager_ class is an abstract class that inherits member functions from the `cSimpleModule` class (an OMNeT++ class). In an OMNeT++ simulation, all classes describing simple modules (C++ objects executing some functionality of the simulation) must extent the `cSimpleModule` class. The inherited member functions let the objects (modules) from classes implementing _SlawTripManager_ interact with other modules in a simulation. 

The SlawMatlab and SlawTransNetw classes extend and implement the _SlawTripManager_ class. This means these classes also describe individual walker models, that is, the m-wm and the t-wm, respectively.

The `SlawMobility` class inherits the functionality of the `LineSegmentsMobility` class from the INET framework. These relation is established since SLAW is a mobility model based on line segments and pause times, as the random waypoint (RWP) model.

As the arrow connecting the _SlawTripManager_ box with the `SlawMobility` box suggests, a simulation only includes one module implementing the _SlawTripManager_ abstract class regardless the number of walkers (nodes) in a experiment. This is done in order to save memory since the are several data structures and objects that can be shared among walkers.

The `SelfSimilarWaypointMap`, the `LATPAlgorithm`, the `SpeedModel`, and `the PauseTimeModel`, are classes whose objects help the simple module implementing the _SlawTripManager_ abstract class to compute self-similar least-action walks. Therefore, the modules extending the _SlawTripManager_ class communicate to instances of the aforementioned modules by member function invocation (method invocation), not by messaging o signaling. This is done in such way because of efficiency.

In Slaw++, a observer is an abstract class that specific observers must extend and implement in order to receive information about a phenomenon to be analyzed, e.g., the flight length distribution, or the inter-contact time distribution, or a trip traveled by a given walker. Observers are in charge of gathering data to produce statistics. There are four observers available: the flight length observer, the trip observer, the position observer, and the connectivity observer.

## Initialization process
Simple modules perform initialization tasks by calling the _initialize()_ member function, which is inherited from the `cSimpleModule` class. This member function is used to set simulation parameters of objects up that are not available when the constructor runs. _initialize()_ runs just before the simulation starts executing.

Modules are instanced according to their declaration in the network module. In Slaw++, the first module to be instanced is a trip manager that corresponds to an object implementing the _SlawTripManager_, either a module from `SlawMatlab` or a module from `SlawTransNetw`. This module is followed by the module instanced from the `SlawMobility` class; a walker, properly. Finally, an observer is instanced.

The initialization of the simulator parameters is done as follows:

### Initialization of a trip manager module
 These are the parameters that are setup during the initialization of the `slaw` module:

  * the name of the walker model (`walkerModel`)
  * the number walkers in a simulation (`walkerNum`)
  * the planning degree, which is an input of the LATP algorithm (`planningDegree`)
  * the self-similar map, denoted as `map`, which needs these parameters:
    
    * the clustering radius used to compute waypoint clusters (`clusteringRadius`)
    * the name of the file containing the coordinates (x, y) of the self-similar distributed waypoints (`mapName`)
    * the self-similarity exponent H (`hurstParameter`)

  * The pause-time model (`pauseTimeModel`) which requires the value of these parameters:

    * the pause-time model type: constant, uniform, normal, and bounded Pareto
    * the first parameter `par1`, which could be used to set the first parameter of a pause-time model.
    * the second parameter `par2` which could be used to store the second parameter of the pause-time model. This parameter could be optional depending on the pause-time model.
    * the third parameter `H` only corresponds to the upper limit of a bounded Pareto distribution.

  * The speed model (`speedModel`) which requires the kind of model (constant, uniform, normal, Rhee). It also needs two parameters `par1` and `par2` which are used to establish the values of the speed model.

Once the above parameters are setup, the `slaw` module is able to load a file containing sets of waypoint clusters for each walker if such data are available. These waypoint clusters correspond to the mobility confined areas assigned to each walker. In other case, it is possible to run tasks for setting up such waypoint clusters or compute the initial waypoint of a walker. These tasks are done according to a individual walker model. Thus, the initialization process of both the `SLAWMatlab` and the `SLAWTransNetw` models is different.

In addition to the above mentioned parameters, trip-manager objects from the SLAWMatlab class also set the cluster ratio (`clusterRatio`) and waypoint ratio (`waypointRatio`) up. These variables determines the number of confined areas per walker and the number of waypoints to be visited by a walker, respectively.

The SlawMatlab walker model indicates the number of areas per walker is computed through the following assignation:

```C++
unsigned portion = ceil(map->getNumberOfAreas() / clusterRatio);
```

Immediately after the above assignation, the confined areas are assigned to each walker according to their weight (popularity degree) in such a way that an area is more popular if several waypoints integrates it.

In the case of the `slaw` objects from the `SlawTransNetw` class, the number of confined areas per walker is computed by sampling an discrete uniform probability function U{a, b}. Once the number of areas of each walker is computed, the specific areas per walker are determined according their weight, just as objects from the SlawMatlab class does this task. This IWM does not utilize neither the cluster ratio nor the waypoint ratio since it computes the number of confined areas per walker and the number of waypoints to be visited by a walker following other criteria.

### Initialization process of walker modules

The next modules to be instanced are walkers. The main functionality of walkers is described by the `SlawMobility` class, which inherits from the `cSimpleModule` class the member function _initialize()_. Into this member function, a pointer to the module whose class implements the _SlawTripManager_ is set up.

Then, walkers invoke a member function named `setWalkerState()`, which sets the confined areas, the first list of destinations, and the initial waypoint of a walker. All these parameters are set up according to a given walker model.

The`SlawMatlab` class computes the destination list by determining the areas to be visited by a walker in a trip. These areas are almost the same the _initialization()_ member function of the IWM modules computes. The actual difference is one area is randomly replaced by another one that is drawn without considering its weight.

Then, the number of waypoints that will be visited in a given area are computed by means of the following code line:

```C++
    //aaa is an ugly variable name from the SLAW MATLAB trace generator
    double aaa = double(area->size()) / waypointRatio;
```

If `aaa` < 1.0, i.e., the number of waypoints to be taken from a given area is less than one, then only a waypoint will be randomly drawn from such area. Otherwise, the number of waypoints depends on the fractional part of `aaa`. If this part is less than a sample drawn from a uniform distribution U(0, 1), then the number of waypoints equals the integer part of `aaa` plus one. In other case, number of waypoints to be drawn equals the integer part of `aaa`. This criterion is taken from the original SLAW trace generator.

Finally, the initial waypoint is computed by randomly taken from the destination list a waypoint.

When invoking the _setWalkerStater()_ member function of the `SlawTransNetw` module, a walker gets its set of confined areas, its home waypoint and its first destination list. The first parameter is gotten from the values the walker module computed during its initialization step. The home waypoint of a walker corresponds to a waypoint randomly drawn from the set of confined areas of such walker. Finally, the destination list is computed in two steps. First, the random component of a trip is computed by drawing between 5% and 10% of waypoints from an area not in the set of confined areas of a walker. Then, a maximum of 120 or 150 waypoints are drawn from the confined areas of a walker, except by one area randomly discarded. It is possible that such maximum could not be reached.

#### Initialization of observer modules

The last modules to be initialized are the observers. The most simple initialization process of the observers is the one corresponding to the trip observer, which only reads the sample size. The flight-length observer only established the number of observations to be gathered. In addition, it sets its functionality up by reading whether it has to classify flights into inter-flights or intra-flights or not.

The initialization process of the `Position`, `ContactTime`, and `InterContactTime` observers is more complicated than the corresponding process of the other observers. This fact is due to these observer needs to read the number of nodes to compute the size of the several data structures. In addition, these observers needs the coverage radius of walkers as well as the dimension of the simulation area to divide it into a certain number of squares. Finally, it reads the number of observations to be captured.

## References
Lee, K., Hong, S., Kim, S. J., Rhee, I., & Chong, S.  (2012, April). SLAW: self-similar least-action human walk. _IEEE/ACM Transactions on Networking_, 20(2), 515–529.
