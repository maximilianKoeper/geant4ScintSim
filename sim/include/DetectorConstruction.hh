/*
 * DetectorConstruction.hh
 */

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

namespace sim
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    virtual void ConstructSDandField();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  private:
    G4LogicalVolume *logicDetector;
    G4VPhysicalVolume* physicalDetector1;
    G4LogicalVolume *Tiles;

  protected:
    G4LogicalVolume* fScoringVolume = nullptr;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif
