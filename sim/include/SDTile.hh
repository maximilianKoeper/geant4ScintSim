/*
 * SDTile.hh
 */

#ifndef SDTile_h
#define SDTile_h 1

#include "G4VSensitiveDetector.hh"
#include <map>


class SDTile : public G4VSensitiveDetector{

  public:
    /// Constructor
    SDTile(const G4String SDname);
    /// Destructor
    virtual ~SDTile(){};

    void Initialize(G4HCofThisEvent*) override;

    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

    virtual void EndOfEvent(G4HCofThisEvent*) override;

  private:
    G4int collectionID;
    G4double edep_acc = 0.;

    G4bool verbose;

    std::map<int, double> energy_distr;

};

 #endif
