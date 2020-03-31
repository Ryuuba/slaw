#include "IWalkerModel.h"

Register_Abstract_Class(IWalkerModel);

IWalkerModel::~IWalkerModel() {
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
