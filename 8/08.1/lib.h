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

