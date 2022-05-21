/*
 * Materials.hh
 */


#include "Materials.hh"

CustomMaterial* CustomMaterial::instance = nullptr;

CustomMaterial::CustomMaterial() {
    std::cout << "Create materials" << std::endl;

    auto nist = G4NistManager::Instance();

    // VAKUUM
    VAKUUM = nist->FindOrBuildMaterial("G4_Galactic");

    // LYSO
    std::vector<G4String> LYSO_elements = { "Lu", "Y", "Si", "O" };
    std::vector<G4float> LYSO_fractionElements = { 0.714467891, 0.04033805, 0.063714272, 0.181479788};
    G4double LYSO_density = 7.36*g/cm3;
    LYSO = new G4Material("LYSO",LYSO_density,4);

    for (unsigned int index = 0; index < LYSO_elements.size(); ++index)
    {
       LYSO->AddElement(nist->FindOrBuildElement(LYSO_elements[index]), LYSO_fractionElements[index]);
    }

};

G4Material* CustomMaterial::init(const std::string& name, const G4String& g4name) {
    auto material = G4NistManager::Instance()->FindOrBuildMaterial(g4name);
    if(!material) {
        printf("FATAL: FindOrBuildMaterial('%s') == nullptr\n", g4name.c_str());
        exit(1);
    }
    lut[name] = material;
    return material;
};
