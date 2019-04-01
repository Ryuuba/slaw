#include "ISlawTripManager.h"

Register_Abstract_Class(SlawTripManager);

SlawTripManager::~SlawTripManager() {
  std::cout << "Slaw Trip Manager\n";
  if (map) {
    delete map;
    std::cout << "Destroying map: OK" << '\n';
  }
  if(speedModel) {
    delete speedModel;
    std::cout << "Destroying speed model: OK" << '\n';
  }
  if(pauseTimeModel) {
    delete pauseTimeModel;
    std::cout << "Destroying pausetime model: OK" << '\n';
  }
  std::cout << "SLAW Trip manager was destroyed" << '\n';
}

void SlawTripManager::loadCKFile(char const* name) {
  std::string filename(name);
  std::ifstream ifs(filename.c_str(), std::ifstream::in);
  if (ifs.is_open()) {
    std::string line;
    while(std::getline(ifs, line)) {
        AreaSet C_k;
        std::istringstream iss(line);
        unsigned areaID;
        while (iss >> areaID)
          C_k.push_back(areaID);
        CkSet.push_back(std::move(C_k));
    }
    ifs.close();
  }
  else
    error("Slaw Trip Manager: %s couldn't be opened\n", filename);
}

void SlawTripManager::setMap() {
  double hurstParameter = par("hurstParameter").doubleValue();
  std::string mapName(par("mapName").stringValue());
  double clusteringRadius = par("clusteringRadius");
  map->setMap(mapName, clusteringRadius, hurstParameter);
}

void SlawTripManager::setPauseTimeModel() {
  PauseTimeModelType type = static_cast<PauseTimeModelType>(
    uint8_t(par("pauseTimeModelType"))
  );
  double par1 = par("pausetimeA");
  double par2 = par("pausetimeB");
  double parH = par("pausetimeH");
  pauseTimeModel->setModel(getRNG(0), type, par1, par2, parH);
}

void SlawTripManager::setSpeedModel() {
  SpeedModelType type = static_cast<SpeedModelType>(
    uint8_t(par("speedModelType"))
  );
  double par1 = par("speedA");
  double par2 = par("speedB");
  speedModel->setModel(getRNG(0), type, par1, par2);
}
