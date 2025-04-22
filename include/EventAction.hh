#include <G4UserEventAction.hh>
#include "globals.hh"

#include "AlpideDetector.hh"
#include "AlpideHit.hh"

class EventAction : public G4UserEventAction
{
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

private:
  G4int fIndex = -1;
};