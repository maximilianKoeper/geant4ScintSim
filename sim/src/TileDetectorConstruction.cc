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

#include "SimCfg.hh"

#include "IOManager.hh"


TileDetectorConstruction::TileDetectorConstruction(G4LogicalVolume * _mother){
  mother=_mother;
}

TileDetectorConstruction::~TileDetectorConstruction() {};

G4VPhysicalVolume* TileDetectorConstruction::Construct(){

  // Get custom material manager
  CustomMaterial & materials = CustomMaterial::Instance();

  // Get config manager
  SimCfg & config = SimCfg::Instance();

  // Get IOManager
  IOManager & ioManager = IOManager::Instance();

  G4bool checkOverlaps = config.getInt("/geom_options/checkOverlaps");

  // geometry variables
  G4int    nTilesX             = config.getInt("/geom_options/detector_dimensions/nTilesX");
  G4int    nTilesY             = config.getInt("/geom_options/detector_dimensions/nTilesY");
  G4double TileDimX            = config.getDouble("/geom_options/detector_dimensions/TileDimX")*mm;
  G4double TileDimY            = config.getDouble("/geom_options/detector_dimensions/TileDimY")*mm;
  G4double TileDimZ            = config.getDouble("/geom_options/detector_dimensions/TileDimZ")*mm;
  G4double TileGapX            = config.getDouble("/geom_options/detector_dimensions/TileGapX")*mm;
  G4double TileGapY            = config.getDouble("/geom_options/detector_dimensions/TileGapY")*mm;

  G4double SipmX          = config.getDouble("/geom_options/detector_dimensions/SipmX")*mm;
  G4double SipmY          = config.getDouble("/geom_options/detector_dimensions/SipmY")*mm;
  G4double SipmZ          = config.getDouble("/geom_options/detector_dimensions/SipmZ")*mm;

  G4double PcbZ           = config.getDouble("/geom_options/detector_dimensions/PcbZ")*mm;

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
  G4ThreeVector pos1 = G4ThreeVector(0, 0*cm, 2*cm);

  //
  // Logical Tile Detector Envelope
  //
  G4double dimX = (TileDimX/2+TileGapX) * (nTilesX+0.5);
  G4double dimY = (TileDimY/2+TileGapY) * (nTilesY+0.5);
  G4double dimZ =  (TileDimZ+SipmZ+PcbZ)/2;
  G4VSolid* TileDetectorShape = new G4Box("logicalTileDetectorShape", dimX, dimY, dimZ); //its size

  logicalTileDetector = new G4LogicalVolume(TileDetectorShape,         //its solid
                       materials.VAKUUM,          //its material
                       "logicalTileDetector");           //its name

  auto physicalTileDetector = new G4PVPlacement(0,                      //no rotation
                                                pos1,                   //at position
                                                logicalTileDetector,    //its logical volume
                                                "logicalTileDetector",  //its name
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
                                 -(SipmZ+PcbZ)/2);

      new G4PVPlacement(0,                      //no rotation
                        posCurrent,             //at position
                        logicalTiles,           //its logical volume
                        "Tile",                 //its name
                        logicalTileDetector,    //its mother  volume
                        false,                  //no boolean operation
                        nTileVolume,            //copy number
                        checkOverlaps);         //overlaps checking

      char integer_string[33];
      std::sprintf(integer_string, "%d", nTileVolume);
      ioManager.data_json["geometry"]["tiles"][integer_string]["posX"] = posCurrent[0];
      ioManager.data_json["geometry"]["tiles"][integer_string]["posY"] = posCurrent[1];
      ioManager.data_json["geometry"]["tiles"][integer_string]["posZ"] = posCurrent[2];

      nTileVolume += 1;
    }
  }

  //
  //  SIPMs
  //
  G4VSolid* SIPMShape = new G4Box("SIPMShape", SipmX/2, SipmY/2, SipmZ/2);

  logicalSIPM = new G4LogicalVolume(SIPMShape, materials.SI, "SIPMLogical");

  posCurrent =  G4ThreeVector(0., 0., 0.);
  G4int nSIPMVolume = 0;
  for(int i=0; i < nTilesX; i++){
    for(int j=0; j < nTilesY; j++){
      posCurrent =  G4ThreeVector((TileDimX+TileGapX) * nTilesX/2 - (TileDimX+TileGapX) * (i+0.5),
                                 (TileDimY+TileGapY) * nTilesY/2 - (TileDimY+TileGapY) * (j+0.5),
                                 dimZ-PcbZ-SipmZ/2);

      new G4PVPlacement(0,                      //no rotation
                        posCurrent,             //at position
                        logicalSIPM,           //its logical volume
                        "SIPM",                 //its name
                        logicalTileDetector,    //its mother  volume
                        false,                  //no boolean operation
                        nSIPMVolume,            //copy number
                        checkOverlaps);         //overlaps checking
      nSIPMVolume += 1;
    }
  }

  //
  //  PCB
  //

  G4VSolid* PCBShape = new G4Box("PCBShape", dimX, dimX, PcbZ/2);

  logicalPCB = new G4LogicalVolume(PCBShape , materials.PCB_FR4, "PCBLogical");
  new G4PVPlacement(0,                      //no rotation
                    G4ThreeVector(0., 0., dimZ-PcbZ/2),    //at position
                    logicalPCB,             //its logical volume
                    "PCB",                  //its name
                    logicalTileDetector,    //its mother  volume
                    false,                  //no boolean operation
                    0,                      //copy number
                    checkOverlaps);         //overlaps checking

  return physicalTileDetector;
};
