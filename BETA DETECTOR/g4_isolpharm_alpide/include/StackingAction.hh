#ifndef STACKING_HH
#define STACKING_HH

#include <G4UserStackingAction.hh>
#include <G4RunManager.hh>
#include <G4AntiNeutrinoE.hh>
#include <G4NeutrinoE.hh>
#include <G4VProcess.hh>
#include <G4TouchableHistory.hh>
#include <G4TransportationManager.hh>
#include <G4AnalysisManager.hh>
#include <G4Track.hh>

#include "PrimaryGeneratorAction.hh"

class PrimaryGeneratorAction;

class StackingAction : public G4UserStackingAction
{
public:
    StackingAction();
    ~StackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) override;

private:
    G4AnalysisManager* fAnalysisManager;
};

#endif
