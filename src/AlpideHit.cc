#include "AlpideHit.hh"

#include "G4TouchableHandle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include <G4SystemOfUnits.hh>


G4ThreadLocal G4Allocator<AlpideHit>* AlpideHitAllocator=0;

AlpideHit::AlpideHit()
 : G4VHit(),
   fEnergy(-1.)
{}

AlpideHit::~AlpideHit() {}

AlpideHit::AlpideHit(const AlpideHit& right)
  : G4VHit()
{
  fEnergy         = right.fEnergy;
  fDepositedEnergy= right.fDepositedEnergy;
  fDepositedEnergyPixel0 = right.fDepositedEnergyPixel0;
  //fShortPixNo     = right.fShortPixNo;
  fxPosition      = right.fxPosition;
}

const AlpideHit& AlpideHit::operator=(const AlpideHit& right)
{
    fEnergy         = right.fEnergy;
    fDepositedEnergy= right.fDepositedEnergy;
    //fShortPixNo     = right.fShortPixNo;
    fDepositedEnergyPixel0 = right.fDepositedEnergyPixel0;
    fxPosition      = right.fxPosition;
  return *this;
}

G4int AlpideHit::operator==(const AlpideHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}


// Find pixel position
void AlpideHit::SetPosition(G4Step *aStep)
{
  const G4TouchableHandle TH = aStep->GetPreStepPoint()->GetTouchableHandle();

  // Retrieve hit physical volumes
  G4VPhysicalVolume *phyPixel = TH->GetVolume();
  G4VPhysicalVolume *phyColumn = TH->GetVolume(1);
  G4VPhysicalVolume *phyMatrix = TH->GetVolume(2);

  // x position (column inside matrix)
  G4ThreeVector posColumn = phyColumn->GetTranslation() + phyMatrix->GetTranslation();

  // z position (along column)
  G4ThreeVector posPixel = phyPixel->GetTranslation() + posColumn;

  fxPosition = posColumn[0];
  fzPosition = posPixel[2];
}