This is the code I (Vittoria Pavanello) developed during my Master's thesis journey to run GEANT4 simulations that evaluate the performance of a MAPS-based beta radiation detector. Order is definitely not a strength of this project and my coding style is probably extremely naive. I beg pardon to everyone that ends up here, but sharing will help progress the work I started during my thesis.  

Inside these folders you can find:

- ALPIDEmeasurments: all the instructions and scripts I used to carry out the first ALPIDE measurments with laboratory sources. The data I collected during the experiments are ALL contained in the 'data' folder;

- BETA DETECTOR: contains ALL the Geant4 simulation, i.e. the 2D in-vitro scenario one, the 3D in-vitro scenario one and the simulation that models the experimental set-up used in the preliminary ALPIDE test;

- DataTakingALPIDE: is a mess. It contains many data I collected during the first raw trails with the ALPIDE that I did not use for the thesis and again the scripts to make the ALPIDE work with the FPGA;

- Schemi: as you may guess, contains some scheamtic illustrations that I used for my thesis. Not too interesting for anyone else I guess.  

BETA DETECTOR SIMULATION

These folders contain all the simulation and analysis files developed to test the future experimental scenario in ISOLPHARM in-vitro experiments using Monte Carlo simulations with GEANT4. The goal is to replicate the response of the beta detector that will be constructed using MAPS ALPIDE when used to study the distribution of the prototype radiopharmaceutical in two- and three-dimensional cell cultures. The detector aims to generate activity maps that reveal where the drug has been internalized through the detection of beta radiation.
To replicate the in-vitro scenarios that will be met, the simulations are divided based on the biological source analyzed. The model of the detector employed in each simulation is the same, but the radiation source is varied based on the necessities.

- 'petri_g4_isolpharm_alpide' contains the simulation that models a two-dimensional traditional cell culture;

- 'g4_isolpharm_alpide' contains the simulation that models a three-dimensional hydrogel-based cell culture;

- 'testALPIDE_g4' contains the simulation that models the experimental scenario used in the first preliminary tests with the ALPIDE chip including sealed laboratory radiation sources.

BETA DETECTOR IN 3D IN-VITRO ENVIRONMENT

These folders contain all the simulation and analysis files developed to test the future experimental scenario in ISOLPHARM in-vitro experiments using Monte Carlo simulations with GEANT4. The goal is to replicate the response of the beta detector that will be constructed using MAPS ALPIDE when used to study the distribution of the prototype radiopharmaceutical in three-dimensional cell cultures. The detector aims to generate activity maps that reveal where the drug has been internalized through the detection of beta radiation.
! These scripts focus on the scenario of HYDROGEL-BASED 3D cell culture.
! The GUI struggles to display all the pixels of the detector and currently takes time to visualize the configuration with 4 ALPIDEs. To make the most of the graphical display, it is important to reduce the number of pixels per side.

The included folders are:

-analysis: This folder contains all the Python scripts used to analyze and interpret the results of Monte Carlo simulations.

-build: This folder is where the GEANT4 simulation is built and contains the produced executable.

-include: In this folder, you'll find all the necessary header files for the GEANT4 simulation.

-macros: This folder contains macros used to run the MC simulation without opening the graphical window.

-outputs: This is the folder where all the outputs of the conducted simulations have been and will be saved.

-src: This is the source folder with the main files used by GEANT4.


SOME USEFUL INSTRUCTIONS TO DEAL WITH THE SIMULATION
Many of the adjustable parameters in the simulation need to be modified within the header file. Here are some suggestions that may expedite your search:

- To change the number of ALPIDEs and their arrangement in the detector, you can adjust the variables 'fNAlpidesAlongShort' and 'fNAlpidesAlongLong' inside 'DetectorConstruction.hh.' Once modified, you should check some lines of code within the source file to modify certain commented lines (it can be helpful to look for the following text: 'if(fNAlpidesAlongShort * fNAlpidesAlongLong == 4));'

- To change the number of cells within the culture, you need to adjust 'fNsources' inside 'DetectorConstruction.hh' and 'PrimaryGeneratoAction.hh.' The number must match to avoid any issues.

- The number of cells in the culture is closely related to how they are arranged within it. They can be enclosed in a confined cluster, randomly dispersed within the volume selected for the cluster (by setting 'is_smallVolume=true' in 'DetectorConstruction.hh'). They can be enclosed in different clusters of varying sizes (by setting 'is_randomPositions=true' in 'DetectorConstruction.hh'). Finally, they can be randomly scattered throughout the entire available volume (when 'is_smallVolume' and 'is_randomPositions' are false).

- The energy threshold to assign to the detector is regulated (in keV) through the variable 'energyTHRESHOLD' in 'AlpideDetector.hh;

- To modify the number of pixels present in a single ALPIDE, you can adjust the variables 'fAlpidePixShort' and 'fAlpidePixLong' inside 'DetectorConstruction.hh.'. This information is useful to visualize the detector through the GUI: reducing the number of pixel per ALPIDE significantly reduces the time required to visualize the setup (typically I set fAlpidePixShort = 512/64 and fAlpidePixLong = 1024/64).