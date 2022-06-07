/*
 * IOManager.cc
 */

#include "IOManager.hh"


IOManager* IOManager::instance = nullptr;

IOManager::IOManager(const std::string& filename) {
  outFile.open(filename.c_str(), std::fstream::trunc | std::fstream::out);
}


void IOManager::WriteOut() {
  outFile << data_json;
  outFile.close();
}
