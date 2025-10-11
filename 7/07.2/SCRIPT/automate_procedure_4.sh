#!/bin/bash

# Script per automatizzare la procedura di equilibrazione e poi di esecuzione
# delle simulazioni MD(NVE) e MC(NVT)

# Creo le cartelle per salvare i file di output
mkdir -p ../OUTPUT_FINALI_4/OUTPUT_MD ../OUTPUT_FINALI_4/OUTPUT_MC

# Procedura di equilibrazione per MD(NVE)
# Copia il file di input per l'equilibrazione in input.dat che il codice legge
# Copia il file di configurazione iniziale fcc in config.xyz che il codice legge
# Esegui il codice
# Copia i file di output in OUTPUT_MD
echo "Equilibrazione MD(NVE)"

cp INPUT/input_eq_MD_4.dat ../INPUT/input.dat
cp ../INPUT/CONFIG/config.fcc ../INPUT/CONFIG/config.xyz
../SOURCE/simulator.exe
#Non salvo i file di equilibrazione

#Procedura di simulazione per MD(NVE)
echo "MD(NVE)"

cp INPUT/input_MD_4.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.xyz ../INPUT/CONFIG/config.xyz
cp ../OUTPUT/CONFIG/conf-1.xyz ../INPUT/CONFIG/conf-1.xyz
../SOURCE/simulator.exe
cp ../OUTPUT/potential_energy.dat ../OUTPUT_FINALI_4/OUTPUT_MD/potential_energy_MD.dat
cp ../OUTPUT/kinetic_energy.dat ../OUTPUT_FINALI_4/OUTPUT_MD/kinetic_energy_MD.dat
cp ../OUTPUT/temperature.dat ../OUTPUT_FINALI_4/OUTPUT_MD/temperature_MD.dat
cp ../OUTPUT/pressure.dat ../OUTPUT_FINALI_4/OUTPUT_MD/pressure_MD.dat
cp ../OUTPUT/gofr.dat ../OUTPUT_FINALI_4/OUTPUT_MD/gofr_MD.dat

rm ../INPUT/CONFIG/conf-1.xyz
rm ../INPUT/CONFIG/config.xyz

# Procedura di equilibrazione per MC(NVT)
# Stampo l'accettazione per controllare che sia del 50%

echo "Equilibrazione MC(NVT)"

cp INPUT/input_eq_MC_4.dat ../INPUT/input.dat
cp ../INPUT/CONFIG/config.fcc ../INPUT/CONFIG/config.xyz
../SOURCE/simulator.exe
#Non salvo i file di equilibrazione

#Procedura di simulazione per MC(NVT)
echo "MC(NVT)"

cp INPUT/input_MC_4.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.xyz ../INPUT/CONFIG/config.xyz
cp ../OUTPUT/CONFIG/conf-1.xyz ../INPUT/CONFIG/conf-1.xyz
../SOURCE/simulator.exe
cp ../OUTPUT/potential_energy.dat ../OUTPUT_FINALI_4/OUTPUT_MC/potential_energy_MC.dat
cp ../OUTPUT/kinetic_energy.dat ../OUTPUT_FINALI_4/OUTPUT_MC/kinetic_energy_MC.dat
cp ../OUTPUT/pressure.dat ../OUTPUT_FINALI_4/OUTPUT_MC/pressure_MC.dat
cp ../OUTPUT/gofr.dat ../OUTPUT_FINALI_4/OUTPUT_MC/gofr_MC.dat
cp ../OUTPUT/acceptance.dat ../OUTPUT_FINALI_4/OUTPUT_MC/acceptance_MC.dat

rm ../INPUT/CONFIG/conf-1.xyz
rm ../INPUT/CONFIG/config.xyz
rm ../INPUT/input.dat