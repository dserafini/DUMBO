#include "AlpideHit.hh"

#include "G4TouchableHandle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include <G4SystemOfUnits.hh>


G4ThreadLocal G4Allocator<AlpideHit>* AlpideHitAllocator=0;

AlpideHit::AlpideHit()
 : G4VHit()
{}

AlpideHit::~AlpideHit() {}

AlpideHit::AlpideHit(const AlpideHit& right)
  : G4VHit()
{
  fDepositedEnergy= right.fDepositedEnergy;
}

const AlpideHit& AlpideHit::operator=(const AlpideHit& right)
{
    fDepositedEnergy= right.fDepositedEnergy;
  return *this;
}

G4int AlpideHit::operator==(const AlpideHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}