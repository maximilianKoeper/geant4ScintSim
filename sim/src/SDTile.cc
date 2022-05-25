/*
 * SDTile.cc
 */

#include "SDTile.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDTile::SDTile(const G4String SDname) : G4VSensitiveDetector(SDname){
  //collectionName.insert("SDTile");
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDTile::Initialize(G4HCofThisEvent* hce){};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SDTile::ProcessHits(G4Step *step, G4TouchableHistory *){
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4double edep = step->GetTotalEnergyDeposit();
  if(edep == 0){return true;};

  G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
  G4cout << copyNo << " edep: " << edep << G4endl;

  return true;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
