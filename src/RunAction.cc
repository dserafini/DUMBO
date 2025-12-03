#include "RunAction.hh"
#include <G4AnalysisManager.hh>
#include "DetectorConstruction.hh"
#include <G4EventManager.hh>

RunAction::RunAction(DetectorConstruction* det) :
  G4UserRunAction(),
  fDetectorConstruction(det)
{
}


void RunAction::BeginOfRunAction(const G4Run*)
{
  G4cout << "RunAction::BeginOfRunAction" << G4endl;
  // Register and create histograms
  
  auto man = G4AnalysisManager::Instance();
  // Bidimensional histo containing the hits
  // man->SetFirstHistoId(1);
  // Create H2
  fIdHitMap = man->CreateH2("hitMap", "Hits in Pixel Array",
      fDetectorConstruction->GetzTotalNumberBins(), - fDetectorConstruction->GetzDetector(), fDetectorConstruction->GetzDetector(),
      fDetectorConstruction->GetxTotalNumberBins(), - fDetectorConstruction->GetxDetector(), fDetectorConstruction->GetxDetector());

  // Histo containing the deposited energy in single pixel
  // Create H2 to retrieve the energy deposited in the pixels
  fIdEnPix = man->CreateH1("enPixMap", "Deposited energy in Pixel", 200, 0, 20); // 0 - 0.1 keV, one bin every 0.0001 keV

  // Histo containing the deposited energy in single pixel
  // Create H2 to retrieve the energy deposited in the pixels
  fIdHitNum = man->CreateH1("HitPixNumber", "Number of Hit Pixels per event", 20, 0, 20); // 0 - 20 number of observed hits per event

  // Ntuple containing total number of particles detected per run
  man->CreateNtuple("PartPerRun", "Number of particles detectde per run");
  man->CreateNtupleIColumn("Particles");
  man->FinishNtuple();

  man->CreateNtuple("PixelHits", "eventID, eDepKeV, x, z");
  man->CreateNtupleIColumn("eventID");      // 0
  man->CreateNtupleDColumn("eDepKeV"); //  1
  man->CreateNtupleDColumn("x");    //  2
  man->CreateNtupleDColumn("z");    //  3
  man->CreateNtupleDColumn("isFirst");  //  4
  man->CreateNtupleDColumn("fstPx");    //  5
  man->CreateNtupleDColumn("fstPy");    //  6
  man->CreateNtupleDColumn("fstPz");    //  7
  man->CreateNtupleDColumn("fstIx");    //  8
  man->CreateNtupleDColumn("fstIy");    //  9
  man->CreateNtupleDColumn("fstIz");    //  10
  man->CreateNtupleDColumn("fstEkin");    //  11
  man->CreateNtupleDColumn("pixID");    //  12
  man->FinishNtuple();

  man->CreateNtuple("Generated", "eventID, eGenKeV, x, z");
  man->CreateNtupleIColumn("eventID");      // 0
  man->CreateNtupleDColumn("eGenKeV"); //  1
  man->CreateNtupleDColumn("x");    //  2
  man->CreateNtupleDColumn("z");    //  3
  man->FinishNtuple();

  

  // man->OpenFile("../outputs/output.root"); // FILE NAME
  man->OpenFile(); // FILE NAME for macro setting
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