#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include "AlpideHit.hh"

class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{

public:
    // Main method that has to be overridden in all detectors
    G4VPhysicalVolume* Construct() override;

    // Create sensitivity
    void ConstructSDandField() override;

    // Constructor and destructor.
    DetectorConstruction();
    ~DetectorConstruction() = default;

    // Helping functions.
    G4int GetPixAlongShort() const      { return fPixAlongShort; }
    G4int GetPixAlongLong()  const      { return fPixAlongLong; }

    G4double GetxDetector () const      {return fShortSide;}
    G4double GetzDetector () const      {return fLongSide;}

    G4double GetxTotalNumberBins () const      {return fNAlpidesAlongShort * fAlpidePixShort;}
    G4double GetzTotalNumberBins () const      {return fNAlpidesAlongLong * fAlpidePixLong;}

private:

    // World volume
    G4LogicalVolume* worldLog;

    // Alpide parameters 
    G4int fAlpidePixShort = 512; // Number of pixels along the ALPIDE's short side
    G4int fAlpidePixLong = 1024; // Number of pixels along the ALPIDE's long side
    G4double fAlpidePixLongSize = 28 * um; 
    G4double fAlpidePixShortSize = 28 * um; 
    G4double fAlpideThickness = 50 * um;
    // Depletion layer thickness (coinciding with the sensitive pixel's region volume)
    G4double fDepletionThickness = 25 * um;
    // Configuration parameters (2x2 or 4x2)
    G4int fNAlpidesAlongShort = 2; // Number of ALPIDE detectors along their short side
    G4int fNAlpidesAlongLong = 2; // Number of ALPIDE detectors along their long side
    // Calculated internally
    G4int fPixAlongShort = -1;
    G4int fPixAlongLong = -1;
    G4double fShortSide = -1.;
    G4double fLongSide = -1.;
    // Oxide layer parameters
    G4double fOxideThickness = 11 * um;
    G4double fOxideCenter = fDepletionThickness / 2 + fOxideThickness / 2;
    G4Material* SiO_2;
    // Non-sensitive layer
    G4double fNonSensitiveThickness = fAlpideThickness - fDepletionThickness - fOxideThickness;
    G4double fNonSensitiveCenter = - (fDepletionThickness / 2 + fNonSensitiveThickness / 2);
    // Dead volume 
    G4double fDeadShortSide = 1.2 * mm;
    G4double fDeadCenter = - (fAlpideThickness / 2 - fOxideThickness - fDepletionThickness / 2);

    // Mylar parameters
    G4double fMylThickness = 50 * um;
    G4double fAlThickness = 5 * um;
    G4double fEFFMylThickness = fMylThickness - fAlThickness;
    G4double fMylarCenter = fDepletionThickness / 2 + fOxideThickness + fEFFMylThickness / 2;
    G4double fAluminumCenter = fDepletionThickness / 2 + fOxideThickness + fEFFMylThickness + fAlThickness / 2;

    // Hydrogel parameters
    G4double fGelSize = 3 * cm; 
    G4double fGelThickness = 500 * um;
    G4double fGelCenter = fDepletionThickness / 2 + fOxideThickness + fMylThickness + fGelThickness/2;
    // Hydrogel material
    G4Material* GelMa;
    G4Material* scaffoldMaterial;

    // Cell parameters
    G4bool is_smallVolume = false;
    G4bool is_randomPositions = false;
    G4int fNsources = 1;
    G4double fBoundaryx = 5 * mm; // Size (x,z) of the reduced volume where to deposit the cells 
    G4double fBoundaryz = 5 * mm;
    G4double fcellRadius = 10 * um; // https://reader.elsevier.com/reader/sd/pii/S0169409X18300024?token=E8618C9C6782273C068EEC57777794B675E24278B543CBBE3C758A982C64E215634CD9EEE368BD3FA478DE83183DCBBF&originRegion=eu-west-1&originCreation=20230514074344
    G4double fminDistance = 2 * fcellRadius; // Minimal distance between two spheres

    // PCB parameters 
    G4double fSupportShortSide = 5 * cm; 
    G4double fSupportLongSide = 10 * cm;
    G4double fSupportThickness = 1 * mm; 
    G4double fSupportCenter = - (fSupportThickness / 2 + fAlpideThickness / 2);
    G4double fSupportLateralCenter = - (fAlpideThickness / 2 - fOxideThickness - fDepletionThickness / 2);
    // PCB material
    G4Material* FR4;

    // Material definition function
    void DefineMaterial();
    // PCB construction function
    void DetectorSupportConstruction2x2();
    void DetectorSupportConstruction4x2();

};

#endif