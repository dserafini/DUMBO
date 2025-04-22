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
{

  AlpideDetector* alpideDetector = new AlpideDetector("alpideDetector", "AlpideHitsCollection");

  if(fIndex < 0)
  {
    fIndex = G4SDManager::GetSDMpointer()
        ->GetCollectionID("alpideDetector/AlpideHC");
    G4cout << "Detector fIndex: " << fIndex << G4endl;
  }


  G4HCofThisEvent* hc = ev->GetHCofThisEvent();
  AlpideHitsCollection* HitsColl = 0;
  if(hc) HitsColl = (AlpideHitsCollection*) hc->GetHC(fIndex);
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  if(HitsColl)
  {
    int numberHits = HitsColl->entries();
    for(int i = 0; i < numberHits; ++i)
    {
        AlpideHit* hit = (*HitsColl)[i];

        /*// Hit histogram
        if (hit->GetDepositedEnergy() > 0) // ENERGY THRESHOLD
        {
          man->FillH2(1, hit->GetHitPixelzPosition(), hit->GetHitPixelxPosition());
        }

        // Single Pixel energy deposit histogram
        if (hit->GetDepositedEnergyPixel0() != 0.25)
        {
          man->FillH1(2, hit->GetDepositedEnergyPixel0());
        }

        // Single Pixel energy deposit histogram
        if (hit->GetDepositedEnergy() > 0)
        {
          man->FillH1(3, alpideDetector->GetNumberOfHitPerEvent());
          //G4cout << hit->GetNumberOfHitPerEvent() << G4endl;
        }*/
    }
  }
}
