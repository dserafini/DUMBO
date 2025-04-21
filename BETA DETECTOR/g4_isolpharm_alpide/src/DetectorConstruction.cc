#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4Box.hh>
#include <G4SDManager.hh>
#include <G4Orb.hh>
#include <Randomize.hh>
#include <G4UnionSolid.hh> 
#include <G4AssemblyVolume.hh>

#include "DetectorConstruction.hh"
#include "AlpideDetector.hh"


DetectorConstruction::DetectorConstruction()
{
    // Single ALPIDE chip creation
    fPixAlongShort = fAlpidePixShort;
    fPixAlongLong = fAlpidePixLong;
    
    fShortSide = fPixAlongShort * fAlpidePixShortSize;
    fLongSide = fPixAlongLong * fAlpidePixLongSize;
}

void DetectorConstruction::DefineMaterial()
{
    G4NistManager *nist = G4NistManager::Instance();

    // Hydrogel material
    // Gelatin
    G4Material* Gelatin = new G4Material("Gelatin", 1.3 * g/cm3, 4); // Pelle maiale gelatina SICMA 
    Gelatin->AddElement(nist->FindOrBuildElement("C"), 35); 
    Gelatin->AddElement(nist->FindOrBuildElement("H"), 34);
    Gelatin->AddElement(nist->FindOrBuildElement("N"), 13);
    Gelatin->AddElement(nist->FindOrBuildElement("O"), 12);
    // Methacrylic Anhydride
    G4Material* MethacrylicAnhydride = new G4Material("MethacrylicAnhydride", 1.0 * g/cm3, 3);
    MethacrylicAnhydride->AddElement(nist->FindOrBuildElement("C"), 8); 
    MethacrylicAnhydride->AddElement(nist->FindOrBuildElement("H"), 10);
    MethacrylicAnhydride->AddElement(nist->FindOrBuildElement("O"), 3);
    // GelMa (Gelatin + Methacrylic Anhydride)
    GelMa = new G4Material("GelMa", 1.1 * g/cm3, 2); 
    GelMa->AddMaterial(Gelatin, 0.67);
    GelMa->AddMaterial(MethacrylicAnhydride, 0.33);
    // Scaffold solution (GelMa + aqueous solution)
    scaffoldMaterial = new G4Material("scaffoldMaterial", 1 * g/cm3, 2); 
    scaffoldMaterial->AddMaterial(GelMa, 0.05);
    scaffoldMaterial->AddMaterial(nist->FindOrBuildMaterial("G4_WATER"), 0.95);

    // Oxide material : silicon dioxide
    SiO_2 = new G4Material("SiO2", 2.196 * g/cm3, 2);
    SiO_2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO_2->AddElement(nist->FindOrBuildElement("O"), 2);

    // Support (PCB) material : FR4
    // Epoxy -> Bisphenol A (BPA)  https://www.sciencedirect.com/science/article/pii/B0080431526002400
    // Wikipedia
    G4Material* Epoxy = new G4Material("Epoxy", 1.217 * g/cm3, 3);
    Epoxy->AddElement(nist->FindOrBuildElement("H"), 16);
    Epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
    Epoxy->AddElement(nist->FindOrBuildElement("O"), 2);
    // FR4 (SiO2 + Epoxy)
    FR4 = new G4Material("FR4", 1.86 * g/cm3, 2);
    FR4->AddMaterial(Epoxy, 0.4);
    FR4->AddMaterial(SiO_2, 0.6);
    
}

void DetectorConstruction::DetectorSupportConstruction2x2()
{    
    G4NistManager *nist = G4NistManager::Instance();

    // SUPPORT VOLUME CONSTRUCTION
    // 1) Base
    G4VSolid* suppBox = new G4Box("suppBox", fSupportShortSide / 2, fSupportThickness / 2, fSupportLongSide / 2);
    G4LogicalVolume *suppLog = new G4LogicalVolume(suppBox, FR4, "suppLog");
    
    // 2-3) Horizontal lines
    G4double fSupportHorizontalShortSide = fSupportShortSide / 2 - fShortSide - fDeadShortSide;
    G4VSolid* hSuppBox = new G4Box("hSuppBox", fSupportHorizontalShortSide / 2, fAlpideThickness / 2, fSupportLongSide / 2);
    G4LogicalVolume *hSuppLog = new G4LogicalVolume(hSuppBox, FR4, "hSuppLog");
    
    // 4-5) Vertical line
    G4double fSupportVerticalLongSide = fSupportLongSide / 2 - fLongSide;
    G4VSolid* vSuppBox = new G4Box("vSuppBox", fShortSide + fDeadShortSide, fAlpideThickness / 2, fSupportVerticalLongSide / 2);
    G4LogicalVolume *vSuppLog = new G4LogicalVolume(vSuppBox, FR4, "vSuppLog");
    

    // AVENGERS ASSEMBLE
    G4AssemblyVolume* assemblePhy = new G4AssemblyVolume();
    std::vector <G4ThreeVector> suppPositions {G4ThreeVector(0, fSupportCenter, 0), 
                                               G4ThreeVector(fSupportHorizontalShortSide / 2 + fShortSide + fDeadShortSide, fSupportLateralCenter, 0), G4ThreeVector(-(fSupportHorizontalShortSide / 2 + fShortSide + fDeadShortSide), fSupportLateralCenter, 0),
                                               G4ThreeVector(0, fSupportLateralCenter, fLongSide + fSupportVerticalLongSide / 2), G4ThreeVector(0, fSupportLateralCenter, -(fLongSide + fSupportVerticalLongSide / 2))};
    assemblePhy->AddPlacedVolume(suppLog, suppPositions[0], 0);
    assemblePhy->AddPlacedVolume(hSuppLog, suppPositions[1], 0);
    assemblePhy->AddPlacedVolume(hSuppLog, suppPositions[2], 0);
    assemblePhy->AddPlacedVolume(vSuppLog, suppPositions[3], 0);
    assemblePhy->AddPlacedVolume(vSuppLog, suppPositions[4], 0);

    G4ThreeVector placement(0, 0, 0);
    assemblePhy->MakeImprint(worldLog, placement, 0);
        
    // Support Color settings
    G4Colour deepGreen(28./255., 121./255., 93./255., 1.);
    G4VisAttributes *visSupp = new G4VisAttributes(deepGreen);
    visSupp->SetForceSolid(true);
    suppLog->SetVisAttributes(visSupp);
    hSuppLog->SetVisAttributes(visSupp);
    vSuppLog->SetVisAttributes(visSupp);
    
}

void DetectorConstruction::DetectorSupportConstruction4x2()
{    
    G4NistManager *nist = G4NistManager::Instance();

    // SUPPORT VOLUME CONSTRUCTION
    // 1) Base
    G4VSolid* suppBox = new G4Box("suppBox", fSupportShortSide / 2, fSupportThickness / 2, fSupportLongSide / 2);
    G4LogicalVolume *suppLog = new G4LogicalVolume(suppBox, FR4, "suppLog");
    
    // 2-3) Horizontal lines (along x, where there are the short sides)
    G4double fSupportHorizontalShortSide = fSupportLongSide / 2 - fLongSide;
    G4VSolid* hSuppBox = new G4Box("hSuppBox", fSupportLongSide / 2, fAlpideThickness / 2, fSupportHorizontalShortSide / 2);
    G4LogicalVolume *hSuppLog = new G4LogicalVolume(hSuppBox, FR4, "hSuppLog");
    
    // 4-5) Vertical line (along z, where there are the long sides)
    G4double fSupportVerticalShortSide = fSupportShortSide / 2 - 2 * fShortSide;
    G4VSolid* vSuppBox = new G4Box("vSuppBox", fSupportVerticalShortSide / 2, fAlpideThickness / 2, fSupportLongSide / 2 - fSupportHorizontalShortSide);
    G4LogicalVolume *vSuppLog = new G4LogicalVolume(vSuppBox, FR4, "vSuppLog");
    

    // AVENGERS ASSEMBLE
    G4AssemblyVolume* assemblePhy = new G4AssemblyVolume();
    std::vector <G4ThreeVector> suppPositions {G4ThreeVector(0, fSupportCenter, 0), 
                                               G4ThreeVector(0, fSupportLateralCenter, fLongSide + fSupportHorizontalShortSide / 2), G4ThreeVector(0, fSupportLateralCenter, -(fLongSide + fSupportHorizontalShortSide / 2)),
                                               G4ThreeVector(2 * fShortSide + fSupportVerticalShortSide / 2, fSupportLateralCenter, 0), G4ThreeVector(-(2 * fShortSide + fSupportVerticalShortSide / 2), fSupportLateralCenter,  0)};
    assemblePhy->AddPlacedVolume(suppLog, suppPositions[0], 0);
    assemblePhy->AddPlacedVolume(hSuppLog, suppPositions[1], 0);
    assemblePhy->AddPlacedVolume(hSuppLog, suppPositions[2], 0);
    assemblePhy->AddPlacedVolume(vSuppLog, suppPositions[3], 0);
    assemblePhy->AddPlacedVolume(vSuppLog, suppPositions[4], 0);

    G4ThreeVector placement(0, 0, 0);
    assemblePhy->MakeImprint(worldLog, placement, 0);
        
    // Support Color settings
    G4Colour deepGreen(28./255., 121./255., 93./255., 1.);
    G4VisAttributes *visSupp = new G4VisAttributes(deepGreen);
    visSupp->SetForceSolid(true);
    suppLog->SetVisAttributes(visSupp);
    hSuppLog->SetVisAttributes(visSupp);
    vSuppLog->SetVisAttributes(visSupp);
    
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();
    DefineMaterial();

    //////////////////////////////////////////////////////////////
    // World volume
    //////////////////////////////////////////////////////////////

    // Set the world dimensions, bigger as it is easier.
    G4double worldSizeX = 20 * cm;
    G4double worldSizeY = 20 * cm;
    G4double worldSizeZ = 20 * cm;

    G4VSolid* worldBox = new G4Box("world", worldSizeX / 2, worldSizeY / 2, worldSizeZ / 2);
    worldLog = new G4LogicalVolume(worldBox, nist->FindOrBuildMaterial("G4_AIR"), "world");
    G4VisAttributes* visAttr = new G4VisAttributes();
    visAttr->SetVisibility(false);
    worldLog->SetVisAttributes(visAttr);
    G4VPhysicalVolume* worldPhy = new G4PVPlacement(nullptr, {}, worldLog, "world", nullptr, false, 0);

    
    //////////////////////////////////////////////////////////////
    // Device volume
    //////////////////////////////////////////////////////////////
    // Create each single ALPIDE as replica of a unit pixel: first as a column, then as a plane of pixels
    G4VSolid* pixBox = new G4Box("pixBox", fAlpidePixShortSize / 2, fDepletionThickness / 2, fAlpidePixLongSize / 2);
    G4VSolid* colBox = new G4Box("colBox", fAlpidePixShortSize / 2, fDepletionThickness / 2, fLongSide / 2);
    G4VSolid* matBox = new G4Box("matBox", fShortSide / 2, fDepletionThickness / 2, fLongSide / 2);
    G4VSolid* oxBox = new G4Box("oxBox", fShortSide / 2, fOxideThickness / 2, fLongSide / 2); // SiO2 layer over the sensitive layer
    G4VSolid* siBox = new G4Box("siBox", fShortSide / 2, fNonSensitiveThickness / 2, fLongSide / 2); // Si insensitive bottom layer

    // Logical volumes
    G4LogicalVolume* pixLog = new G4LogicalVolume(pixBox, nist->FindOrBuildMaterial("G4_Si"), "pixLog");
    G4LogicalVolume* sensitiveVolume = nullptr;
    sensitiveVolume = pixLog;
    G4LogicalVolume* colLog = new G4LogicalVolume(colBox, nist->FindOrBuildMaterial("G4_Si"), "colLog");
    // SiO2 top layer 
    G4LogicalVolume* oxLog = new G4LogicalVolume(oxBox, SiO_2, "oxLog");
    // Non-sensitive silicon bottom layer
    G4LogicalVolume* siLog = new G4LogicalVolume(siBox, nist->FindOrBuildMaterial("G4_Si"), "siLog");

    // REPLICA ALONG COLUMN
    new G4PVReplica("pixPhy",                               
                    pixLog,                                 // Logical volume to place
                    colLog,                                 // Logical volume of mother
                    kZAxis,                                 // Axis where replicas are placed
                    fPixAlongLong,                          // How many replicas to place
                    fAlpidePixLongSize);                    // Pitch for the replicas
    
    // Matrix replicas variables
    std::vector<G4ThreeVector> matPositions;
    // ALPIDE positions when they are in 4x2 or 2x2 configuration
    if(fNAlpidesAlongShort * fNAlpidesAlongLong == 8)
    {
        G4double zSpacingLong = 3 * fLongSide/ 2; // Center shift along x-axis (longest distance)
        G4double zSpacingShort = fLongSide / 2; // Center shift along x-axis
        G4double xSpacing = fShortSide / 2; // Center shift along z-axis
        matPositions  = {G4ThreeVector(-xSpacing, 0 , zSpacingLong), G4ThreeVector(-xSpacing, 0 , -zSpacingLong),
                         G4ThreeVector(xSpacing, 0 , zSpacingLong), G4ThreeVector(xSpacing, 0 , -zSpacingLong),
                         G4ThreeVector(-xSpacing, 0 , zSpacingShort), G4ThreeVector(-xSpacing, 0 , -zSpacingShort),
                         G4ThreeVector(xSpacing, 0 , zSpacingShort), G4ThreeVector(xSpacing, 0 , -zSpacingShort)};
    }
    else 
    {   
        G4double xSpacing = fShortSide / 2; // Center shift along x-axis
        G4double zSpacing = fLongSide / 2; // Center shift along z-axis
        matPositions = {G4ThreeVector(-xSpacing, 0 , zSpacing), G4ThreeVector(xSpacing, 0 , zSpacing),
                        G4ThreeVector(-xSpacing, 0 , -zSpacing), G4ThreeVector(xSpacing, 0 , -zSpacing)};
    }

    // Matrix Logical/PhysicalVolume vectors                                          
    std::vector <G4LogicalVolume*> matLog (fNAlpidesAlongShort*fNAlpidesAlongLong);
    std::vector <G4VPhysicalVolume*> matReplica (fNAlpidesAlongShort*fNAlpidesAlongLong);
    std::vector <G4VPhysicalVolume*> matPhy (fNAlpidesAlongShort*fNAlpidesAlongLong);
    // Oxide layer vectors
    std::vector <G4VPhysicalVolume*> oxPhy (fNAlpidesAlongShort*fNAlpidesAlongLong);
    // Non-sensitive silicon layer vectors
    std::vector <G4VPhysicalVolume*> siPhy (fNAlpidesAlongShort*fNAlpidesAlongLong);

    // Detector Color settings
    G4Colour lightOrange(185./255., 128./255., 122./255., 1.);
    G4VisAttributes *visMat = new G4VisAttributes(lightOrange);
    visMat->SetForceSolid(true);
    siLog->SetVisAttributes(visMat);
    // Oxide color settings
    G4Colour Yellow(255./255., 255./255., 0./255., 0.7);
    G4VisAttributes *visOx = new G4VisAttributes(Yellow);
    visOx->SetForceSolid(true);
    oxLog->SetVisAttributes(visOx);

    for (G4int i = 0; i < fNAlpidesAlongShort*fNAlpidesAlongLong; ++i) 
    {
        matLog[i] = new G4LogicalVolume(matBox, nist->FindOrBuildMaterial("G4_Si"), "matLog"+std::to_string(i));
        matLog[i]->SetVisAttributes(visMat);
        matReplica[i] = new G4PVReplica("colPhy"+std::to_string(i),     
                        colLog,                                         // Logical volume to place
                        matLog[i],                                      // Logical volume of mother
                        kXAxis,                                         // Axis where replicas are placed
                        fPixAlongShort,                                 // How many replicas to place
                        fAlpidePixShortSize);                           // Pitch for the replicas

        matPhy[i] = new G4PVPlacement(0,                                //no rotation
                    matPositions[i],                                    //center position
                    matLog[i],                                          //its logical volume
                    "matPhy"+std::to_string(i),                         //its name
                    worldLog,                                           //its mother  volume
                    false,                                              //no boolean operation
                    i);                                                 //copy number.
        
        // Oxide placement
        oxPhy[i] = new G4PVPlacement(0, matPositions[i] + G4ThreeVector(0, fOxideCenter, 0), oxLog, "oxPhy"+std::to_string(i), worldLog, false, 0);
        siPhy[i] = new G4PVPlacement(0, matPositions[i] + G4ThreeVector(0, fNonSensitiveCenter, 0), siLog, "siPhy"+std::to_string(i), worldLog, false, 0);
    }

    //////////////////////////////////////////////////////////////
    // Dead layers
    //////////////////////////////////////////////////////////////
    // Construction of the unsensitive electronics volume that each ALPIDE has along one of its long sides
    // !!! These layer are implemented ONLY for the 2x2 configuration !!!
    if(fNAlpidesAlongShort * fNAlpidesAlongLong == 4)
    {
        G4VSolid* deadBox = new G4Box("deadBox", fDeadShortSide / 2, fAlpideThickness / 2, fLongSide / 2);
        G4LogicalVolume *deadLog = new G4LogicalVolume(deadBox, nist->FindOrBuildMaterial("G4_Si"), "deadLog");    

        // AVENGERS ASSEMBLE
        G4AssemblyVolume* assemblePhy = new G4AssemblyVolume();
        std::vector <G4ThreeVector> suppPositions {G4ThreeVector(fShortSide + fDeadShortSide / 2, fDeadCenter, fLongSide / 2), G4ThreeVector(fShortSide + fDeadShortSide / 2, fDeadCenter, -fLongSide / 2),
                                                   G4ThreeVector(-(fShortSide + fDeadShortSide / 2), fDeadCenter, fLongSide / 2), G4ThreeVector(-(fShortSide + fDeadShortSide / 2), fDeadCenter, -fLongSide / 2)};
        assemblePhy->AddPlacedVolume(deadLog, suppPositions[0], 0);
        assemblePhy->AddPlacedVolume(deadLog, suppPositions[1], 0);
        assemblePhy->AddPlacedVolume(deadLog, suppPositions[2], 0);
        assemblePhy->AddPlacedVolume(deadLog, suppPositions[3], 0);

        G4ThreeVector placement(0, 0, 0);
        assemblePhy->MakeImprint(worldLog, placement, 0);

        // Support Color settings
        G4Colour graydead(160./255., 160./255., 160./255., 1.);
        G4VisAttributes *visDead = new G4VisAttributes(graydead);
        visDead->SetForceSolid(true);
        deadLog->SetVisAttributes(visDead);
    }

    //////////////////////////////////////////////////////////////
    // Detector Support volume
    //////////////////////////////////////////////////////////////
    if(fNAlpidesAlongShort * fNAlpidesAlongLong == 4) DetectorSupportConstruction2x2();
    //if(fNAlpidesAlongShort * fNAlpidesAlongLong == 8) DetectorSupportConstruction4x2();


    //////////////////////////////////////////////////////////////
    // Mylar volume (substrate between detector and gel)
    //////////////////////////////////////////////////////////////
    G4VSolid *mylBox = new G4Box("mylarBox", fSupportShortSide / 2, fEFFMylThickness / 2, fSupportLongSide / 2);
    G4VSolid *alBox = new G4Box("aluminumBox", fSupportShortSide / 2, fAlThickness / 2, fSupportLongSide / 2);

    G4LogicalVolume *mylLog = new G4LogicalVolume(mylBox, nist->FindOrBuildMaterial("G4_MYLAR"), "mylarLog");
    G4LogicalVolume *alLog = new G4LogicalVolume(alBox, nist->FindOrBuildMaterial("G4_Al"), "aluminumLog");

    G4Colour gray(250./255., 250./255., 250./255., .2);
    G4VisAttributes *visMy = new G4VisAttributes(gray);
    visMy->SetForceSolid(true);
    mylLog->SetVisAttributes(visMy);
    G4Colour darkGray(87./255., 87./255., 87./255., .4);
    G4VisAttributes *visAl = new G4VisAttributes(darkGray);
    visAl->SetForceSolid(true);
    alLog->SetVisAttributes(visAl);

    G4VPhysicalVolume *mylPhy = new G4PVPlacement(0, G4ThreeVector(0, fMylarCenter, 0), mylLog, "mylarPhy", worldLog, false, 0);
    G4VPhysicalVolume *alPhy = new G4PVPlacement(0, G4ThreeVector(0, fAluminumCenter, 0), alLog, "aluminumPhy", worldLog, false, 0);


    //////////////////////////////////////////////////////////////
    // Hydrogel volume
    //////////////////////////////////////////////////////////////
    G4VSolid *gelBox = new G4Box("gelBox", fGelSize / 2, fGelThickness / 2, fGelSize / 2);
    G4LogicalVolume *gelLog = new G4LogicalVolume(gelBox, scaffoldMaterial, "gelLog");
    G4VPhysicalVolume *gelPhy = new G4PVPlacement(0, G4ThreeVector(0, fGelCenter, 0), gelLog, "gelPhy", worldLog, false, 0);


    //////////////////////////////////////////////////////////////
    // Cells volume
    //////////////////////////////////////////////////////////////
    // Construction and placement of cells. Cells can be randomly dispered or confined in one single volume with adjustable size 
    // (when is_smallVolume=true) or confined in many different clusters of adjustable size (is_randomPositions=true)
    G4ThreeVector bMin, bMax; // Bounding box of the gel box vectors
    gelBox->BoundingLimits(bMin, bMax); // BoundingLimits calculates the minimum and maximum coordinates of a box that encloses the entire solid
    std::vector<G4ThreeVector> position; // Positions vector for cell sources

    G4double radius10sqrt10 = sqrt(fGelSize * fGelSize / (1000 * M_PI)); // Radius computed to have a 1% cluster volume when considering 10 clusters
    std::vector<G4double> radiusVector = {radius10sqrt10 * 3, radius10sqrt10 * 2, radius10sqrt10, radius10sqrt10 * 6/7, radius10sqrt10 * 5/7,
                                          radius10sqrt10 * 2/3, radius10sqrt10 * 3/5, radius10sqrt10 * 3/7, radius10sqrt10 * 2/5, radius10sqrt10 * 1/3};
    std::vector<std::pair<G4double, G4double>> centersVector = { {-5, 1.94}, {-10, -7.80}, {2.77, -2.38}, {0.66, 5.92}, {3.55, -6.66}, {-12.32, -0.1}, {-13.18, 2.81},
                                                                 {5.06, 4.11}, {8.99, 5.77}, {11.58, -6.00} };

    // Custom PDF to have more cells in bigger clusters when is_randomPositions=true
    G4double probabilities[] = {0.30, 0.20, 0.10, 0.086, 0.072, 0.066, 0.060, 0.043, 0.040, 0.033};
    G4RandGeneral* randomGenerator = new G4RandGeneral(probabilities, 10, 0);

    while (position.size() < fNsources) 
    {
        G4ThreeVector p;
        G4double x,y,z;
        G4int randomVolumeID;

        // IF particles are randomly distributed within the whole hydrogel volume
        if (is_smallVolume == false && is_randomPositions == false)
        {
            x = bMin.x() + (bMax.x() - bMin.x())*G4UniformRand();
            y = bMin.z() + (bMax.z() - bMin.z())*G4UniformRand();
            z = bMin.y() + (bMax.y() - bMin.y())*G4UniformRand();
            p.set(x, z, y); // space point p
        }

        // IF the cells are grouped in single small volume (is_smallVolume == true) or in single clusters (is_randomPositions == true)
        else
        {
            // Choose random volume between the 10 clusters with custom pdf
            randomVolumeID = static_cast<G4int>(randomGenerator->shoot() * 10); // Random number in [0, 9] to select the radius of the volume and its position (with custom PDF)

            G4double radius;
            // If is_randomPositions, consider one of the radii collected in radiusVector (then randomized).
            if (is_randomPositions == true) radius = radiusVector[randomVolumeID] * G4UniformRand();
            // If is_smallVolume, the size of the cell deposition volume becomes 1% of total volume
            if (is_smallVolume == true) radius = sqrt(fGelSize * fGelSize / (100 * M_PI)) * G4UniformRand();

            // Random angle
            G4double angle = 2 * M_PI * G4UniformRand();
            x = radius * cos(angle);
            y = radius * sin(angle);
            z = bMin.y() + (bMax.y() - bMin.y()) * G4UniformRand();

            // Final calculated space point as center of the cell
            p.set(x, z, y); // space point p
        }
                                                                                                  
        // Check if the position is inside the volume                                                                           
        if (gelBox->Inside(p) != kInside) continue;
        // Check if the distance of the point p is enough distant from the volume surface.
        if (gelBox->DistanceToOut(p) < fcellRadius) continue;

        // Correct y position accounting for translation in volume position (BoundingLimits calculates the limits as if the volume is centered in (0,0,0))
        z += fGelCenter;
        if (is_smallVolume == true) {x += 0.3*mm; y -= 0.8*mm;}

        // IF (is_randomPositions == true) correct x,z positions accounting for translation of the cluster's centers
        if (is_randomPositions == true)
        {
            x += centersVector[randomVolumeID].second; y += centersVector[randomVolumeID].first;
        }

        G4ThreeVector cell_point(x, z, y);
        // If only ONE cell, set direcly the position in the middle of the medium
        if (fNsources == 1) cell_point = G4ThreeVector(0, fGelCenter, 0);

        // The 2 cells is a special case that can be studied with the following code
        /*
        G4ThreeVector cell_point1(x, y, z);
        G4ThreeVector cell_point2(x, y, z);
        if (fNsources == 2)
        {
            cell_point1 = G4ThreeVector(0, fGelCenter, 0.25*mm);
            cell_point2 = G4ThreeVector(0, fGelCenter, -0.25*mm);

            position.push_back(cell_point1);
            position.push_back(cell_point2);
        }*/

        G4int numberOfPositions = position.size();
        G4int k = 0;
        for ( ; k < numberOfPositions; ++k) {
          if ((position[k] - cell_point).mag() < fminDistance) break;
          // For each position position[k] in the vector, the loop calculates the distance between
          // the current point p and the position in the vector. If the distance is less than a minimum distance fminDistance, the loop breaks
        }
        if (k == numberOfPositions) position.push_back(cell_point); // add space point p to the list of positions (if p was found enough distanced  
                                                                    // from the surface and from other points)                                
    }

    delete randomGenerator;

    G4Orb *cellOrb = new G4Orb("cellOrb", fcellRadius);
    G4LogicalVolume *cellLog = new G4LogicalVolume(cellOrb, nist->FindOrBuildMaterial("G4_WATER"), "cellLog");
    G4Colour lightViolet(237./255., 223./255., 255./255., 1.);
    G4VisAttributes *visCell = new G4VisAttributes(lightViolet);
    visCell->SetForceSolid(true);
    cellLog->SetVisAttributes(visCell);

    // Physical volume definition for source cells
    std::vector <G4VPhysicalVolume*> cellPhy;
    cellPhy.reserve(fNsources); // re-elaborate this way to initialize the vector
    for (G4int i = 0; i < fNsources; ++i) 
    {
        cellPhy[i] = new G4PVPlacement(0, position[i], cellLog, "Cell_"+std::to_string(i), worldLog, false, 0);
    }
    

    // The Construct() method has to return the final (physical) world volume:
    return worldPhy;

}


void DetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->SetVerboseLevel(2);

    // Create the custom sensitive detector developed.
    AlpideDetector* alpideDetector = new AlpideDetector("alpideDetector", "AlpideHC");

    G4SDManager* sdMan = G4SDManager::GetSDMpointer();
    sdMan->AddNewDetector(alpideDetector);

    SetSensitiveDetector("pixLog", alpideDetector);    
}