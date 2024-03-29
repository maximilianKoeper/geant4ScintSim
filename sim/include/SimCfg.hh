/*
 * SimCfg.hh
 */

#ifndef SimCfg_h
#define SimCfg_h 1

#include "globals.hh"
#include <nlohmann/json.hpp>


//using json = nlohmann::json;

///
/// Configuration class
///

class SimCfg {

  private:
    static SimCfg* instance;

  public:
    static SimCfg& Instance() {
        return *instance;
    };
    static SimCfg& Instance(const std::string& filename) {
        if(!instance) instance = new SimCfg(filename);
        return *instance;
    };


    double getDouble(const char *name);
    int getInt(const char *name);
    std::string getString(const char *name);
    bool getBool(const char *name);

    nlohmann::json getCfg(){return cfg;};

  private:
    /// Protected constructor to ensure singleton nature
    SimCfg(const std::string& filename);

    nlohmann::json cfg;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
