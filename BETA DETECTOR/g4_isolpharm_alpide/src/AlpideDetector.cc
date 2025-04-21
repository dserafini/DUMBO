#include "AlpideDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

#include "G4VProcess.hh"
#include <vector>

// Custom function to identify if a number is present more than once in a vector (strumentopolo useful later)
G4int numberOfDifferentElementsInVector(const std::vector<G4String>& vector) {
  if (vector.empty()) {
    return 0; // If vector is empty return 0
  }

  G4String firstElement = vector[0];
  for (size_t i = 1; i < vector.size(); i++) {
    if (vector[i] != firstElement) {
      return 2;  // If an element is different from another return 2
    }
  }

  return 1; // If all elements are the same return 1
}


AlpideDetector::AlpideDetector(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}


AlpideDetector::~AlpideDetector()
{}


void AlpideDetector::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new AlpideHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Initialize counts of hits for each event
  hitEventCount = 0;
  stepCounter = 0;

  // Energy deposited per hit
  depositedEnergy = 0;
}


// This function processes one hits: for each step inside the sensitive volume, different information is collected 
// (energy, hit pixel in the detector, type of interacting particle)
G4bool AlpideDetector::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
  // Skip if the process is not defined, or it will segfault.
  if(!aStep->GetPreStepPoint()->GetProcessDefinedStep()) return false;

  const G4TouchableHandle TH = aStep->GetPreStepPoint()->GetTouchableHandle();
  G4VPhysicalVolume *phyHitVolume = TH->GetVolume();

  // With the following line it is possible to checke ONLY the contribution of electrons or gammas
  //if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() != "gamma") return false;

  AlpideHit* newHit = new AlpideHit();

  // Get the incoming kinetic energy
  newHit->SetTrackEnergy(aStep->GetTrack()->GetKineticEnergy());
  // Set the hit position
  newHit->SetPosition(aStep);

  // Save pixel copy numbers
  pixCopyNumber = TH->GetVolume()->GetCopyNo();
  pixelCopyNumber.push_back(pixCopyNumber);

  // Get pixel hit by specific step position
  // Retrieve hit physical volumes
  G4VPhysicalVolume *phyPixel = TH->GetVolume();
  G4VPhysicalVolume *phyColumn = TH->GetVolume(1);
  G4VPhysicalVolume *phyMatrix = TH->GetVolume(2);
  // x position (column inside matrix)
  G4ThreeVector posColumn = phyColumn->GetTranslation() + phyMatrix->GetTranslation();
  // z position (along column)
  G4ThreeVector posPixel = phyPixel->GetTranslation() + posColumn;

  // Find particle type
  particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();

  // SAVE INFO PER HIT: to do that, check the step's parameters not to save wrong info
  
  // IF it is the first step inside the sensitive volume (i.e. stepCounter == 0)
  if (stepCounter == 0) 
  {
    depositedEnergy += 1000 * aStep->GetTotalEnergyDeposit(); // update energy in keVs
    fxPosition = posColumn[0]; // Retrieve pixel position along detector's short side
    fzPosition = posPixel[2]; // Retrieve pixel position along detector's long side
  }

  // IF it is not the first step inside the sensitive volume (i.e. stepCounter != 0) 
  // AND the previous step happened in the SAME sensitive pixel (i.e. pixelCopyNumber[stepCounter-1] == pixelCopyNumber[stepCounter])
  if (stepCounter != 0 && pixelCopyNumber[stepCounter-1] == pixelCopyNumber[stepCounter]) 
  {
    depositedEnergy += 1000 * aStep->GetTotalEnergyDeposit();// update energy in keVs (keep adding for each step inside a fixed pixel to understand how much energy was released inside that)
    fxPosition = posColumn[0];
    fzPosition = posPixel[2];
  }

  // IF it is not the first step inside a fixed sensitive pixel (i.e. stepCounter != 0) 
  // AND the previous step happened in a DIFFERENT sensitive pixel (i.e. pixelCopyNumber[stepCounter-1] != pixelCopyNumber[stepCounter])
  if (stepCounter != 0 && pixelCopyNumber[stepCounter-1] != pixelCopyNumber[stepCounter]) 
  {
    if (depositedEnergy > energyTHRESHOLD)// IF energy threshold requirments are met
    { 
      hitsVector.push_back(std::make_pair(fzPosition, fxPosition)); // Save (z,x) hit pixel position inside a vector
      depositedEnergyPerHit.push_back(depositedEnergy); // Save energy
      interactingParticles.push_back(particleName); // Save particle type name
      hitEventCount ++; // Increase the number of observed hits per analyzed event
    }

    depositedEnergy = 0; // Reset energy to 0 to start analyzing new pixel 
    depositedEnergy += 1000 * aStep->GetTotalEnergyDeposit();
    fxPosition = posColumn[0];
    fzPosition = posPixel[2];
  }

  stepCounter ++; // Increase step counter (variable to count how many steps are happening)

  // Add to the HC
  fHitsCollection->insert(newHit);

  return true;

}

// What happens at the end of the events
void AlpideDetector::EndOfEvent(G4HCofThisEvent*)
{
  // SAVE DATA AT THE END OF THE EVENT
  if (depositedEnergy > energyTHRESHOLD) // IF energy threshold requirments are met
  {
    hitEventCount ++; // Increase the number of observed hits per analyzed event
    // Save data
    hitsVector.push_back(std::make_pair(fzPosition, fxPosition));
    depositedEnergyPerHit.push_back(depositedEnergy);
    interactingParticles.push_back(particleName);
  }

  // SAVE HOW MANY PARTICLES WERE DETECTED
  if (numberOfDifferentElementsInVector(interactingParticles) == 2) // IF there is more than one type of particle (e- or gamma) insieme interactingParticles vector
  {
    // Save total of 2 interacting particles
    interactingParticleCount ++;
    interactingParticleCount ++;
  }
  if (numberOfDifferentElementsInVector(interactingParticles) == 1)
  {
    // Save total of 1 interacting particle
    interactingParticleCount ++;
  }


  // SAVING HISTOGRAMS AND NTUPLE (declared in RunAction.cc)
  if(hitEventCount != 0)
  {
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillH1(3, hitEventCount);
    for (auto energy: depositedEnergyPerHit) man->FillH1(2, energy);
    for (auto hit: hitsVector) man->FillH2(1, hit.first, hit.second);
    // Nutple
    man->AddNtupleRow();
    man->FillNtupleIColumn(0, interactingParticleCount);
  }

  // RESET VARIABLES AT THE END OF EVENT
  pixelCopyNumber.clear();
  depositedEnergyPerHit.clear();
  hitsVector.clear();
  interactingParticles.clear();
  hitEventCount = 0;
  stepCounter = 0;
  depositedEnergy = 0;

}
