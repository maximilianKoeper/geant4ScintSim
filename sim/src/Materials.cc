/*
 * Materials.cc
 */


#include "Materials.hh"

#include "G4UserEventAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

CustomMaterial* CustomMaterial::instance = nullptr;

CustomMaterial::CustomMaterial() {
    std::cout << "Create materials" << std::endl;

    auto nist = G4NistManager::Instance();

    // -----------------------
    // VAKUUM
    VAKUUM = nist->FindOrBuildMaterial("G4_Galactic");

    // -----------------------
    // Si
    SI = nist->FindOrBuildMaterial("G4_Si");

    // -----------------------
    // WOLFRAM
    WOLFRAM = nist->FindOrBuildMaterial("G4_W");

    // -----------------------
    // PCB Material - FR4
    //Epoxy (for FR4 )
    G4double Epoxy_density = 1.2*g/cm3;
    G4Material* Epoxy = new G4Material("Epoxy" , Epoxy_density, 2);
    Epoxy->AddElement(nist->FindOrBuildElement("H"), 2);
    Epoxy->AddElement(nist->FindOrBuildElement("C"), 2);
    //SiO2 (Quarz)
    G4Material* SiO2 = new G4Material("SiO2", 2.200*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
    //FR4 (Glass + Epoxy)
    G4double FR4_density = 1.86*g/cm3;
    PCB_FR4 = new G4Material("FR4" , FR4_density, 2);
    PCB_FR4->AddMaterial(Epoxy, 0.472);
    PCB_FR4->AddMaterial(SiO2, 0.528);

    // -----------------------
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
