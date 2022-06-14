/*
 * IOManager.cc
 */

#include "IOManager.hh"


IOManager* IOManager::instance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Construct a new IOManager::IOManager object
 * 
 * @param filename 
 */
IOManager::IOManager(const std::string& filename) {
  outFile.open(filename.c_str(), std::fstream::trunc | std::fstream::out);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Write json data to file
 * 
 */
void IOManager::WriteOut() {
  outFile << data_json;
  outFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
