#ifndef RUNACTION_HH
#define RUNACTION_HH

#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include <G4ParticleDefinition.hh>
#include <G4Accumulable.hh>
#include "AlpideHit.hh"

class DetectorConstruction;

class RunAction : public G4UserRunAction
{
public:
  //! constructor
  RunAction(DetectorConstruction*);

  //! destructor
  ~RunAction();

  //! Main interface
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);


private:

  // I want a pointer to DetectorConstuction to keep track of pixel number.
  DetectorConstruction*  fDetectorConstruction;

};

#endif
