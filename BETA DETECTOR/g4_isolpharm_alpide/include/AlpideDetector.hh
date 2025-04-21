
#ifndef AlpideDetector_h
#define AlpideDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "AlpideHit.hh"

#include <vector>
#include <utility>
#include <G4AnalysisManager.hh>

class G4Step;
class G4HCofThisEvent;

class AlpideDetector : public G4VSensitiveDetector
{
  public:
    AlpideDetector(const G4String& name,
                const G4String& hitsCollectionName);
    virtual ~AlpideDetector();
  
    // Methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

    G4int hitEventCount;
    G4int stepCounter;
    // Pixel copynumber
    G4int pixCopyNumber;
    // Copy number vector
    std::vector<G4int> pixelCopyNumber;

    // Deposited energy per hit
    G4double depositedEnergy;
    std::vector<G4double> depositedEnergyPerHit;

    // Hit position
    G4double fxPosition;
    G4double fzPosition;
    std::vector<std::pair<G4double, G4double>> hitsVector;

    // Particle interacting with the detector
    G4int interactingParticleCount;
    G4String particleName;
    std::vector<G4String> interactingParticles;

    // THRESHOLD (keV)
    G4double energyTHRESHOLD = 1;

    // Get the total number of interacting particles
    G4int GetinteractingParticleCount() const       { return interactingParticleCount; }


  private:
    AlpideHitsCollection* fHitsCollection;

};

#endif //AlpideDetector
