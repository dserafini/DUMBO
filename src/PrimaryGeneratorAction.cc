#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleGun.hh>
#include <Randomize.hh>
#include <G4IonTable.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4RandomDirection.hh>
#include <G4GeneralParticleSource.hh>

#include "PrimaryGeneratorAction.hh"

using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // GPS generation
    //fGPS = new G4GeneralParticleSource();

    // ParticleGun generation
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // GPS
    //fGPS->GeneratePrimaryVertex(anEvent);

    // ParticleGun (Ag-111)
    G4int Z = 47, A = 111; // Ag-111
    G4double ionCharge = 0.*eplus;
    G4double excitEnergy = 0.*keV;

    G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, excitEnergy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(ionCharge);
    fParticleGun->SetParticleEnergy(0.*GeV);

    // Retrieve a random cell volume
    G4PhysicalVolumeStore *PVStore = G4PhysicalVolumeStore::GetInstance();
    G4int rand = (int)(G4UniformRand() * fNsources);
    auto PV = PVStore->GetVolume("Cell_" + std::to_string(rand));

    // Define randomized source position (inside cell)
    G4ThreeVector center = PV->GetObjectTranslation();
    G4ThreeVector source = center + G4RandomDirection() * fcellRadius;

    // Set ParticleGun start position inside cells
    fParticleGun->SetParticlePosition(source);

    // Primary vertex generation
    fParticleGun->GeneratePrimaryVertex(anEvent);

}
