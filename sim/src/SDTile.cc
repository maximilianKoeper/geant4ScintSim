/*
 * SDTile.cc
 */

#include "SDTile.hh"
#include "G4UnitsTable.hh"
//#include <fstream>
//#include <nlohmann/json.hpp>
#include "G4RunManager.hh"

#include "IOManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDTile::SDTile(const G4String SDname) : G4VSensitiveDetector(SDname){
  //collectionName.insert("SDTile");
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDTile::Initialize(G4HCofThisEvent* hce){
  edep_acc = 0;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SDTile::ProcessHits(G4Step *step, G4TouchableHistory *){
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4double edep = step->GetTotalEnergyDeposit();
  if(edep == 0){return true;};

  edep_acc += edep;

  G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
  //G4cout << copyNo << " edep: " << G4BestUnit(edep,"Energy") << G4endl;

  return true;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDTile::EndOfEvent(G4HCofThisEvent*){
  G4cout << "\n--------------------End of SD Event------------------------" << "\n";
  G4cout << "Total edep in Sensitive Detector Volume: " << G4BestUnit(edep_acc,"Energy") << "\n";
  G4cout << "-----------------------------------------------------------" << G4endl;

  IOManager & ioManager = IOManager::Instance();
  char integer_string[33];
  std::sprintf(integer_string, "%d", G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
  ioManager.data_json["event"][integer_string]["edep"] = edep_acc;

  edep_acc = 0;

};
