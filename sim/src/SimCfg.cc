/*
 * SimCfg.cc
 */


#include "SimCfg.hh"
#include <fstream>


SimCfg* SimCfg::instance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Construct a new Sim Cfg:: Sim Cfg object
 * 
 * @param filename 
 */
SimCfg::SimCfg(const std::string& filename) {
  std::ifstream i(filename.c_str());
  i >> cfg;
  i.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Returns Value from config file
 * 
 * @param name 
 * @return double 
 */
double SimCfg::getDouble(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<double>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Returns Value from config file
 * 
 * @param name 
 * @return int 
 */
int SimCfg::getInt(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<int>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Returns Value from config file
 * 
 * @param name 
 * @return std::string 
 */
std::string SimCfg::getString(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<std::string>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Returns Value from config file
 * 
 * @param name 
 * @return bool 
 */
bool SimCfg::getBool(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<bool>();
}
