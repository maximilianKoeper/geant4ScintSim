/*
 * IOManager.hh
 */

#ifndef IOManager_h
#define IOManager_h 1

#include "globals.hh"
#include <fstream>
#include <nlohmann/json.hpp>

class IOManager {

 private:
   static IOManager* instance;

 public:
   static IOManager& Instance() {
      return *instance;
   };
   static IOManager& Instance(const std::string& filename) {
       if(!instance) instance = new IOManager(filename);
       return *instance;
   };

   //void addEvent(nlohmann::json j){};

   void WriteOut();
   void CreateNewEvent(int eventID);
   bool CloseCurrentEvent();
   bool AddEventInfo(const char *tag, double value);
   nlohmann::json data_json;

 private:
    /// Protected constructor to ensure singleton nature
    IOManager(const std::string& filename);

    std::fstream outFile;
    int currentEventID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
