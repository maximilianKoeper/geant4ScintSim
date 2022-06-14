/*
 * PrimaryGeneratorAction.cc
 */

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "SimCfg.hh"
#include "IOManager.hh"

namespace sim
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="e-");
  fParticleGun->SetParticleDefinition(particle);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  // Get config manager
  SimCfg & config = SimCfg::Instance();
  maxEnergy = config.getDouble("/geom_options/particle_gun_options/momentum_max")*GeV;
  pX = config.getDouble("/geom_options/particle_gun_options/momentum_dir/x");
  pY = config.getDouble("/geom_options/particle_gun_options/momentum_dir/y");
  pZ = config.getDouble("/geom_options/particle_gun_options/momentum_dir/z");

  offsetX = config.getDouble("/geom_options/particle_gun_options/position_offset/x")*mm;
  offsetY = config.getDouble("/geom_options/particle_gun_options/position_offset/y")*mm;
  offsetZ = config.getDouble("/geom_options/particle_gun_options/position_offset/z")*mm;


  fParticleGun->SetParticleEnergy(maxEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

  if (!fEnvelopeBox)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if ( fEnvelopeBox ) {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  }
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  //G4double size = 0.8;
  //G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
  //G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  //G4double z0 = -0.5 * envSizeZ;
  G4double x0 = 0+offsetX;
  G4double y0 = 0+offsetY;
  G4double z0 = -0.5 * envSizeZ+offsetZ;

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(pX,
                                                           pY,
                                                           pZ));

  
  G4double currentParticleEnergy = G4UniformRand()*maxEnergy;
  fParticleGun->SetParticleEnergy(currentParticleEnergy);

  IOManager & ioManager = IOManager::Instance();
  int eventID = anEvent->GetEventID();
  ioManager.data_json["event"][std::to_string(eventID)]["particle_energy"] = currentParticleEnergy;
  
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
