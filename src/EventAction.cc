#include <G4AnalysisManager.hh>
#include <G4SDManager.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4Neutron.hh>

#include "EventAction.hh"
#include "AlpideHit.hh"

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* ev)
{}
