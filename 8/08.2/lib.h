#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "random.h"

struct Parametri{
    double mu;
    double sigma;
};

struct H_avg_err{
    double H_avg;
    double H_err;
};

double error( double av, double av2, int n );
double psi_T ( double x, Parametri p );
double H (double x, Parametri p);
bool Metropolis_x ( double x_new, double x_old, Parametri p, Random &rnd );
bool Metropolis_param ( double H_new, double H_old, double beta, Random &rnd );
H_avg_err H_avg ( Parametri p, Random &rnd, bool print);
void deltaTune ( double &delta, Random &rnd, Parametri p);
Parametri simulatedAnnealing ( double temp, Parametri p_old, Random &rnd );