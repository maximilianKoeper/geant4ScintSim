/*
 * SimCfg.cc
 */


#include "SimCfg.hh"
#include <fstream>


SimCfg* SimCfg::instance = nullptr;

SimCfg::SimCfg(const std::string& filename) {
  std::ifstream i(filename.c_str());
  i >> cfg;
}

int SimCfg::get(const std::string& name){
  return cfg[nlohmann::json::json_pointer(name.c_str())].get<int>();
}

int SimCfg::get(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<int>();
}

double SimCfg::getDouble(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<double>();
}

int SimCfg::getInt(const char *name){
  return cfg[nlohmann::json::json_pointer(name)].get<int>();
}
