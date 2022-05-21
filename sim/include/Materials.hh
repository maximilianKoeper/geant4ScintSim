/*
 * Materials.hh
 */

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

/// Event action class
///


class CustomMaterial {

  private:
    static CustomMaterial* instance;

  public:
    static CustomMaterial& Instance() {
        if(!instance) instance = new CustomMaterial;
        return *instance;
    }

  private:
    /// Protected constructor to ensure singleton nature
    CustomMaterial();

    std::map<std::string, G4Material*> lut;

    G4Material* init(const std::string& name, const G4String& g4name);

  public:
    G4Material* get(const std::string& name) const {
        auto it = lut.find(name);
        if(it == lut.end()) {
            printf("FATAL: unknown material '%s'\n", name.c_str());
            exit(1);
        }
        return it->second;
    }

    G4Material* LYSO;
    G4Material* VAKUUM;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
