/*
 * TileDetectorConstruction.hh
 */

#ifndef TileDetector_h
#define TileDetector_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;

class Tile;


/// Constructing the tile detector
class TileDetectorConstruction {
  public:
      /// Constructor
      TileDetectorConstruction(G4LogicalVolume * mother);


      /// Destructor
      virtual ~TileDetectorConstruction();

      /// Create all the objects of the detector geometry
      virtual G4VPhysicalVolume* Construct();

      G4LogicalVolume * logicalTileDetector;

  protected:
      /// The tile detector logical volume

      /// The logical volume of the tiles
      G4LogicalVolume * logicalTiles;

      G4LogicalVolume * mother;
};

#endif
