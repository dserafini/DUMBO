#include <vector>
#include <ctime>
#include <random>

#include <G4RunManagerFactory.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4String.hh>
#include <G4UImanager.hh>

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"


int main(int argc, char** argv)
{
  G4cout << "Application starting..." << G4endl;

  // Get the current time
  // std::time_t currentTime = std::time(nullptr);
  // Create a random number generator engine
  // std::default_random_engine generator(static_cast<unsigned int>(currentTime));
  // Set the random seed for GEANT4
  // G4Random::setTheEngine(new CLHEP::MTwistEngine(generator()));
  // G4Random::setTheSeed(static_cast<long>(currentTime));

  std::vector<G4String> macros;
  G4bool interactive = false;

  // Parse command line arguments
  if  (argc == 1)
    {
      interactive = true;
    }
  else
    {
      for (int i = 1; i < argc; i++)
        {
	  G4String arg = argv[i];
	  if (arg == "-i" || arg == "--interactive")
            {
	      interactive = true;
	      continue;
            }
	  else
            {
	      macros.push_back(arg);
            }
        }
    }

  // Create the run manager (let the RunManagerFactory decide if MT, 
  // sequential or other). The flags from G4RunManagerType are:  
  // Default (default), Serial, MT, Tasking, TBB
  auto* runManager  = 
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
  runManager->SetVerboseLevel(1);

  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();

  runManager->SetUserInitialization(new PhysicsList());
  
  auto det = new DetectorConstruction();
  runManager->SetUserInitialization(det);
  runManager->SetUserInitialization(new ActionInitialization(det));

  G4UIExecutive* ui = nullptr;
  if (interactive)
    {
      G4cout << "Creating interactive UI session ...";
      ui = new G4UIExecutive(argc, argv);
    }
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  for (auto macro : macros)
    {
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command + macro);
    }

  if (interactive)
    {
      if (ui->IsGUI())
	{
	  UImanager->ApplyCommand("/control/execute macros/ui.mac");
	}
      else
	{
	  UImanager->ApplyCommand("/run/initialize");
	}
      ui->SessionStart();
      delete ui;
    }

  delete runManager;
    
  G4cout << "Application successfully ended.\nBye :-)" << G4endl;

  return 0;
}
