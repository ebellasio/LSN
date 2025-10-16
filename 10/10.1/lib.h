#pragma once

#include "random.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit

using namespace std;
using namespace arma;

// Funzione che calcola la matrice delle distanze tra le città
mat distanze_citta ( int n, char S, Random &rnd);

