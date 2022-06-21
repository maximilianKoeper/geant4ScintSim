/*
 * DetectorConstruction.cc
 */

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "TileDetectorConstruction.hh"
#include "Materials.hh"

#include "G4SDManager.hh"
#include "SDTile.hh"

#include "SimCfg.hh"

namespace sim
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get custom material manager
  CustomMaterial & materials = CustomMaterial::Instance();

  // Get config manager
  SimCfg & config = SimCfg::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 100*cm, env_sizeZ = 50*cm;
  G4Material* env_mat = materials.get("VAKUUM");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = config.getBool("/geom_options/checkOverlaps");

  //
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = materials.get("VAKUUM");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       world_sizeXY, world_sizeXY, world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Envelope
  //
  G4Box* solidEnv =
  new G4Box("Envelope",                    //its name
  env_sizeXY, env_sizeXY, env_sizeZ); //its size

  logicDetector =
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicDetector,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // Absorber
  if (config.getInt("/geom_options/absorber_options/present") != 0){
    G4Box* absorberShape =
    new G4Box("Absorber",                    //its name
              config.getDouble("/geom_options/absorber_options/dimX")/2*mm, 
              config.getDouble("/geom_options/absorber_options/dimY")/2*mm, 
              config.getDouble("/geom_options/absorber_options/dimZ")/2*mm); //its size

    G4Material* absorberMaterial = materials.get(config.getString("/geom_options/absorber_options/material"));

    absorber = new G4LogicalVolume(absorberShape,            //its solid
                        absorberMaterial,             //its material
                        "Absorber");         //its name
    
    new G4PVPlacement(0,                       // rotation
                      G4ThreeVector(0,0,-config.getDouble("/geom_options/absorber_options/distance")*mm),         //at (0,0,0)
                      absorber,                //its logical volume
                      "Absorber",              //its name
                      logicDetector,              //its mother  volume
                      false,                   //no boolean operation
                      0,                       //copy number
                      checkOverlaps);          //overlaps checking
  }
  //
  // Scint Block
  //
  auto* TDC = new TileDetectorConstruction(logicDetector);
  physicalDetector1 = TDC->Construct();
  Tiles = TDC->logicalTiles;

  //
  //always return the physical World
  //
  return physWorld;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField(){

  SDTile* sdtile = new SDTile("SDTile");
  G4SDManager::GetSDMpointer()->AddNewDetector(sdtile);
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  Tiles->SetSensitiveDetector(sdtile);
}

}
