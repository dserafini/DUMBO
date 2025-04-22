#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>

#include "DetectorConstruction.hh"
#include "Parameters.hh"

class G4ParticleGun;
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent) override;

private:
    // GPS generation
    //G4GeneralParticleSource* fGPS;

    // ParticelGun generation
    G4ParticleGun *fParticleGun;

    // DetectorConstruction parameters (check that the parameters in DetectorConstruction.hh are the same)
    // Cells in source
    G4int fNsources = Parameters::nCells;
    G4double fcellRadius = 10 * um;
};

#endif