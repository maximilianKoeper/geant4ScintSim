/*
 * TileConstruction.cc
 */

#include "TileDetectorConstruction.hh"
#include "Materials.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"


TileDetectorConstruction::TileDetectorConstruction(G4LogicalVolume * _mother){
  mother=_mother;
}

TileDetectorConstruction::~TileDetectorConstruction() {}

G4VPhysicalVolume* TileDetectorConstruction::Construct(){

  // Get custom material manager
  CustomMaterial & materials = CustomMaterial::Instance();

  G4bool checkOverlaps = true;

  // geometry variables
  G4int    nTilesX             = 80;
  G4int    nTilesY             = 80;
  G4double TileDimX            = 3*mm;
  G4double TileDimY            = 3*mm;
  G4double TileDimZ            = 15*mm;
  G4double TileGapX            = 0.01*mm;
  G4double TileGapY            = 0.01*mm;

  G4cout << "\n";
  G4cout << "-------------------------------------------";
  G4cout << "Tile dimension is " << TileDimX << " x " << TileDimY << " x " << TileDimZ << " mm3" << "\n";
  G4cout << "Gap between tiles is " << TileGapX << " mm in X and " << TileGapY << " mm in Y direction"  << "\n";
  G4cout << "Tile Detector dimension is " << (TileDimX+TileGapX) * nTilesX << " x " << (TileDimY+TileGapY) * nTilesY << "mm2" << "\n";
  G4cout << "-------------------------------------------" << G4endl;
  //---------------------------------------------------------


  //
  // Scint Block
  //
  //G4ThreeVector pos1 = G4ThreeVector(0, -1*cm, 7*cm);
  G4ThreeVector pos1 = G4ThreeVector(0, 0*cm, 0*cm);
  //
  // Logical Tile Detector Envelope
  //
  G4VSolid* TileDetectorShape =
   new G4Box("logicalTileDetectorShape", (TileDimX/2+TileGapX) * nTilesX + TileGapX, (TileDimY/2+TileGapY) * nTilesY + TileGapY, TileDimZ/2); //its size

  logicalTileDetector = new G4LogicalVolume(TileDetectorShape,         //its solid
                       materials.VAKUUM,          //its material
                       "logicalTileDetector");           //its name

  auto physicalTileDetector = new G4PVPlacement(0,                       //no rotation
                                                pos1,                   //at position
                                                logicalTileDetector,    //its logical volume
                                                "logicalTileDetector",               //its name
                                                mother,                 //its mother  volume
                                                false,                  //no boolean operation
                                                0,                      //copy number
                                                checkOverlaps);         //overlaps checking



  //
  //  Tiles
  //
  G4VSolid* TileShape = new G4Box("tileShape", TileDimX/2, TileDimY/2, TileDimZ/2);

  logicalTiles = new G4LogicalVolume(TileShape , materials.LYSO, "TileEnvelope");

  G4ThreeVector posCurrent =  G4ThreeVector(0., 0., 0.);
  G4int nTileVolume = 0;
  for(int i=0; i < nTilesX; i++){
    for(int j=0; j < nTilesY; j++){
      posCurrent =  G4ThreeVector((TileDimX+TileGapX) * nTilesX/2 - (TileDimX+TileGapX) * (i+0.5),
                                 (TileDimY+TileGapY) * nTilesY/2 - (TileDimY+TileGapY) * (j+0.5),
                                 0);

      new G4PVPlacement(0,                      //no rotation
                        posCurrent,             //at position
                        logicalTiles,           //its logical volume
                        "Tile",                 //its name
                        logicalTileDetector,    //its mother  volume
                        false,                  //no boolean operation
                        nTileVolume,            //copy number
                        checkOverlaps);         //overlaps checking
      nTileVolume += 1;
    }
  }

  return physicalTileDetector;
}
