#!/bin/bash

# Script per automatizzare la procedura Metropolis

# Procedura per T=2.0
# Copia il file di input di T=2.0 in input.dat che il codice legge
# Esegui il codice
# Copia i file di output in OUTPUT_FINALI
echo "T=2.0"
cp INPUT_METRO/input_T2.0.dat ../INPUT/input.dat
../SOURCE/simulator.exe
cp ../OUTPUT/total_energy.dat ../OUTPUT_FINALI/total_energy_metropolis.dat
cp ../OUTPUT/specific_heat.dat ../OUTPUT_FINALI/specific_heat_metropolis.dat
cp ../OUTPUT/susceptibility.dat ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.9
echo "T=1.9"
cp INPUT_METRO/input_T1.9.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.8
echo "T=1.8"
cp INPUT_METRO/input_T1.8.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.7
echo "T=1.7"
cp INPUT_METRO/input_T1.7.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.6
echo "T=1.6"
cp INPUT_METRO/input_T1.6.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.5
echo "T=1.5"
cp INPUT_METRO/input_T1.5.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.4
echo "T=1.4"
cp INPUT_METRO/input_T1.4.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.3
echo "T=1.3"
cp INPUT_METRO/input_T1.3.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.2
echo "T=1.2"
cp INPUT_METRO/input_T1.2.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.1
echo "T=1.1"
cp INPUT_METRO/input_T1.1.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=1.0
echo "T=1.0"
cp INPUT_METRO/input_T1.0.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=0.9
echo "T=0.9"
cp INPUT_METRO/input_T0.9.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=0.8
echo "T=0.8"
cp INPUT_METRO/input_T0.8.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=0.7
echo "T=0.7"
cp INPUT_METRO/input_T0.7.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=0.6
echo "T=0.6"
cp INPUT_METRO/input_T0.6.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat

# Procedura per T=0.5
echo "T=0.5"
cp INPUT_METRO/input_T0.5.dat ../INPUT/input.dat
cp ../OUTPUT/CONFIG/config.spin ../INPUT/config.spin
../SOURCE/simulator.exe
cat ../OUTPUT/total_energy.dat >> ../OUTPUT_FINALI/total_energy_metropolis.dat
cat ../OUTPUT/specific_heat.dat >> ../OUTPUT_FINALI/specific_heat_metropolis.dat
cat ../OUTPUT/susceptibility.dat >> ../OUTPUT_FINALI/susceptibility_metropolis.dat
