#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

//! Optional user classes
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"

#include <G4RadioactiveDecay.hh>
#include <G4IonTable.hh>
#include <G4RadioactivityTable.hh>
#include <G4SystemOfUnits.hh>

ActionInitialization::ActionInitialization(DetectorConstruction* det):
    fDetectorConstruction(det)
    {}

void ActionInitialization::Build() const
{

    SetUserAction(new PrimaryGeneratorAction());
    
    SetUserAction(new RunAction(fDetectorConstruction));

    SetUserAction(new EventAction());
    SetUserAction(new StackingAction());
}