#include "RunAction.hh"
#include <G4AnalysisManager.hh>
#include "DetectorConstruction.hh"
#include <G4EventManager.hh>

RunAction::RunAction(DetectorConstruction* det) :
  G4UserRunAction(),
  fDetectorConstruction(det)
{}


void RunAction::BeginOfRunAction(const G4Run*)
{
  // Register and create histograms
  auto man = G4AnalysisManager::Instance();

  // Bidimensional histo containing the hits
  man->SetFirstHistoId(1);
  // Create H2
  man->CreateH2("hitMap", "Hits in Pixel Array",
      fDetectorConstruction->GetzTotalNumberBins(), - fDetectorConstruction->GetzDetector(), fDetectorConstruction->GetzDetector(),
      fDetectorConstruction->GetxTotalNumberBins(), - fDetectorConstruction->GetxDetector(), fDetectorConstruction->GetxDetector());

  // Histo containing the deposited energy in single pixel
  man->SetFirstHistoId(2);
  // Create H2 to retrieve the energy deposited in the pixels
  man->CreateH1("enPixMap", "Deposited energy in Pixel", 200, 0, 20); // 0 - 0.1 keV, one bin every 0.0001 keV

  // Histo containing the deposited energy in single pixel
  man->SetFirstHistoId(3);
  // Create H2 to retrieve the energy deposited in the pixels
  man->CreateH1("HitPixNumber", "Number of Hit Pixels per event", 20, 0, 20); // 0 - 20 number of observed hits per event

  // Ntuple containing total number of particles detected per run
  man->CreateNtuple("PartPerRun", "Number of particles detectde per run");
  man->CreateNtupleIColumn("Particles");
  man->FinishNtuple();

  man->OpenFile("../outputs/output.root"); // FILE NAME
}

void RunAction::EndOfRunAction(const G4Run*)
{
  // Write and save file properly
  auto man = G4AnalysisManager::Instance();

  man->Write();
  man->CloseFile();
  
}


RunAction::~RunAction()
{}