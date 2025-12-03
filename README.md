# DUMBO simulation

Started from Vittoria Pavanello.

## Scheme

- la superficie del detector (Oxide layer) sta ad altezza y = 23.5 um

- la direzione dall'alto (campione) verso il basso (chip) è 0 -1 0

The order of volumes along y is:

- the sensitive depleted layer has a thickness of 25 um and its center is at zero

- the oxide layer has a thickness of 11 um and stays on top of the sensitive layer: center at + 25 um / 2 + 11 um / 2

- the surface of the alpide is then at + 25 um / 2 + 11 um = 23.5 um

- the non sensitive layer has a thickness of 14 um and stays right below of the sensitive layer: center at - 25 um / 2 - 14 um / 2

- the collimator has a variable thickness, such as 10 mm, and stays on top of the oxide layer: center at + 25 um / 2 + 11 um / 2 + 10 mm / 2

- the pcb support has a thickness of 1 mm and stays below the non sensitive layer: center at - 25 um / 2 - 14 um / 2 - 1 mm / 2

Therefore, without collimator, a point-like source on top of the detector stays at (0,23.5,0) um; with a collimator it stays at (0,10023.5,0) um.

## Useful instructions from the past

Many of the adjustable parameters in the simulation need to be modified within the header file. Here are some suggestions that may expedite your search:

- To change the number of ALPIDEs and their arrangement in the detector, you can adjust the variables 'fNAlpidesAlongShort' and 'fNAlpidesAlongLong' inside 'DetectorConstruction.hh.' Once modified, you should check some lines of code within the source file to modify certain commented lines (it can be helpful to look for the following text: 'if(fNAlpidesAlongShort * fNAlpidesAlongLong == 4));'

- To change the number of cells within the culture, you need to adjust 'fNsources' inside 'DetectorConstruction.hh' and 'PrimaryGeneratoAction.hh.' The number must match to avoid any issues.

- The number of cells in the culture is closely related to how they are arranged within it. They can be enclosed in a confined cluster, randomly dispersed within the volume selected for the cluster (by setting 'is_smallVolume=true' in 'DetectorConstruction.hh'). They can be enclosed in different clusters of varying sizes (by setting 'is_randomPositions=true' in 'DetectorConstruction.hh'). Finally, they can be randomly scattered throughout the entire available volume (when 'is_smallVolume' and 'is_randomPositions' are false).

- The energy threshold to assign to the detector is regulated (in keV) through the variable 'energyTHRESHOLD' in 'AlpideDetector.hh;

- To modify the number of pixels present in a single ALPIDE, you can adjust the variables 'fAlpidePixShort' and 'fAlpidePixLong' inside 'DetectorConstruction.hh.'. This information is useful to visualize the detector through the GUI: reducing the number of pixel per ALPIDE significantly reduces the time required to visualize the setup (typically I set fAlpidePixShort = 512/64 and fAlpidePixLong = 1024/64).