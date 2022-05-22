/*
 * Materials.hh
 */

#ifndef CustomMaterials_h
#define CustomMaterials_h 1

#include "G4NistManager.hh"

///
/// Custom materials class
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
