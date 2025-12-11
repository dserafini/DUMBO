#!/bin/bash

# Timestamp per log: formato YYYYMMDD_HHMMSS
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOGFILE="run_${TIMESTAMP}.log"

# Parametri principali
NEVENTS="100000"  # Numero di eventi per posizione
NEVENTSPROG=$(( NEVENTS / 10 ))

# Intestazione del file di log
echo "=== Log started on $(date) ===" > "$LOGFILE"
echo "=== Log filename: ${LOGFILE} ===" >> "$LOGFILE"

# Definisci le posizioni del GPS
# declare -a POSITIONS_X=(-7.5 -2.5 2.5)
declare -a POSITIONS_X=(0)
# declare -a OUTPUTS=("outputAg1.root" "outputAg2.root" "outputAg3.root")
declare -a OUTPUTS=("outputAg1.root")

for i in "${!POSITIONS_X[@]}"; do
    xpos=${POSITIONS_X[$i]}
    outfile=${OUTPUTS[$i]}
    TEMP_MACRO="macro_temp_pos${i}.mac"

    echo ">>> [START] position X=${xpos} mm"

    ###########################################################################
    # CREA MACRO TEMPORANEA
    ###########################################################################
    cat > "$TEMP_MACRO" <<EOF
/chip/shortSide 512
/chip/longSide 1024

/run/reinitializeGeometry
/run/initialize

# set source
/gps/particle ion
/gps/ion 47 111 0 0.
/gps/energy 0 eV

# set source shape
/gps/ang/type iso
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/pos/radius 2 mm
/gps/pos/rot1 1 0 0
/gps/pos/rot2 0 0 1

# set point-like source
/gps/pos/centre ${xpos} 1 0 mm

/run/printProgress ${NEVENTSPROG}
/analysis/setFileName ${outfile}
/run/beamOn ${NEVENTS}
EOF

    ###########################################################################
    # LANCIA SIMULAZIONE (output → log)
    ###########################################################################
    ./alpideSim "$TEMP_MACRO" >> "$LOGFILE" 2>&1

    echo ">>> [END]   position X=${xpos} mm"

    # Rimuovi macro temporanea
    rm -f "$TEMP_MACRO"

    echo "----------------------------------------------"
done

echo "########### ALL SIMULATIONS COMPLETED ###########"
echo "Log saved to $LOGFILE"
