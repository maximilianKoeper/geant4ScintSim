/*
 * SDTile.hh
 */

#ifndef SDTile_h
#define SDTile_h 1

#include "G4VSensitiveDetector.hh"


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

};

 #endif
