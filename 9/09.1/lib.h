#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit
#include "random.h"

using namespace std;
using namespace arma;

// Funzione che calcola la matrice delle distanze tra le città
mat distanza_citta ( int n, char S, Random &rnd);

