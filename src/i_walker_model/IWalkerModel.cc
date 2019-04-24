#include "IWalkerModel.h"

Register_Abstract_Class(IWalkerModel);

IWalkerModel::~IWalkerModel() {
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
  std::cout << "Walker Model  was destroyed" << '\n';
}

void IWalkerModel::loadCKFile(char const* name) {
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
    error("WalkerModel: %s couldn't be opened\n", filename.c_str());
}

void IWalkerModel::setMap() {
  double hurstParameter = par("hurstParameter").doubleValue();
  std::string mapName(par("mapName").stringValue());
  double clusteringRadius = par("clusteringRadius");
  bool success = map->setMap(mapName, clusteringRadius, hurstParameter);
  if (!success)
    error("WalkerModel: %s could not be loaded\n", mapName.c_str());
}

void IWalkerModel::setPauseTimeModel() {
  PauseTimeModelType type = static_cast<PauseTimeModelType>(
    uint8_t(par("pauseTimeModelType"))
  );
  double par1 = par("pausetimeA");
  double par2 = par("pausetimeB");
  double parH = par("pausetimeH");
  pauseTimeModel->setModel(getRNG(0), type, par1, par2, parH);
}

void IWalkerModel::setSpeedModel() {
  SpeedModelType type = static_cast<SpeedModelType>(
    uint8_t(par("speedModelType"))
  );
  double par1 = par("speedA");
  double par2 = par("speedB");
  speedModel->setModel(getRNG(0), type, par1, par2);
}
