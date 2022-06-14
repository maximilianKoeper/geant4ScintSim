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

void IOManager::CreateNewEvent(int eventID){
  currentEventID = eventID;
  return;
}

bool IOManager::CloseCurrentEvent(){
  currentEventID = -1;
  return true;
}

bool IOManager::AddEventInfo(const char *tag, double value){
  data_json["event"][currentEventID][nlohmann::json::json_pointer(tag)] = value;
  return true;
}

//bool AddEventEdep(int tileID, double edep){
//  return true;
//}