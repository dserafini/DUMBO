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


 G4int GetIdHitMap() const { return fIdHitMap; }
    G4int GetIdEnPix() const { return fIdEnPix; }
    G4int GetIdHitNum() const { return fIdHitNum; }

private:

  // I want a pointer to DetectorConstuction to keep track of pixel number.
  DetectorConstruction*  fDetectorConstruction;
  
  G4int fIdHitMap;
  G4int fIdEnPix;
  G4int fIdHitNum;   
};

#endif
