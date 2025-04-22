#include "PhysicsList.hh"

#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include <G4ProductionCutsTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4Electron.hh>

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{  
  // Standard EM physics
  RegisterPhysics(new G4EmStandardPhysics());
  
  // Decay Physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive Decay Physics
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

void PhysicsList::SetCuts()
{}
