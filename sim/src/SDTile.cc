/*
 * SDTile.cc
 */

#include "SDTile.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"

#include "IOManager.hh"

#include "SimCfg.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDTile::SDTile(const G4String SDname) : G4VSensitiveDetector(SDname){
  collectionName.insert("SDTile");
  SimCfg & config = SimCfg::Instance();
  verbose = config.getBool("/verbose_options/SDTile");
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Initializing sensitive detector and reset edep_acc and energy_distr map
 * 
 * @param hce 
 */
void SDTile::Initialize(G4HCofThisEvent* hce){
  edep_acc = 0;

  energy_distr.clear();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Proccesses hits in sensitive detector. Edep and tileID is saved to energy_distr map.
 * 
 * @param step 
 * @return G4bool 
 */
G4bool SDTile::ProcessHits(G4Step *step, G4TouchableHistory *){
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4double edep = step->GetTotalEnergyDeposit();
  if(edep == 0.){return true;};

  edep_acc += edep;

  G4int copyNo = touchable->GetVolume(0)->GetCopyNo();

  auto itr = energy_distr.find(copyNo);
  if (itr != energy_distr.end()){
       edep += (*itr).second;
       (*itr).second = edep;
  }
  else{
   energy_distr[copyNo] = edep;
  }
  //G4cout << copyNo << " edep: " << G4BestUnit(edep,"Energy") << G4endl;

  return true;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Pushes saved data (energy_distr map) to IOManager.
 * 
 */
void SDTile::EndOfEvent(G4HCofThisEvent*){
  if (verbose){
    G4cout << "\n--------------------End of SD Event------------------------" << "\n";
    G4cout << "Total edep in Sensitive Detector Volume: " << G4BestUnit(edep_acc,"Energy") << "\n";
    G4cout << "-----------------------------------------------------------" << G4endl;
  }

  IOManager & ioManager = IOManager::Instance();
  int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  ioManager.data_json["event"][std::to_string(eventID)]["edep_detector"] = edep_acc;

  for (auto const& [key, val] : energy_distr){
    ioManager.data_json["event"][std::to_string(eventID)]["tileHitEdep"][std::to_string(key)] = val;
  }

  edep_acc = 0;
  energy_distr.clear();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
