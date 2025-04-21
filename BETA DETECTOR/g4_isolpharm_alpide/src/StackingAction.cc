#include "StackingAction.hh"

StackingAction::StackingAction()
{
    fAnalysisManager = G4AnalysisManager::Instance();
}

StackingAction::~StackingAction()
{}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
    // Kill anti-neutrinos
    if (track->GetDefinition() == G4AntiNeutrinoE::AntiNeutrinoE() || track->GetDefinition() == G4NeutrinoE::NeutrinoE())
    {
        return fKill;
    }

    return fUrgent;

}