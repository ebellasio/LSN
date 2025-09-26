/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"

using namespace std;

double error( double av, double av2, int n ){
    if ( n == 0 ) return 0.0;
    else return sqrt( (av2 - av*av ) / double(n) );
};

double f ( double x ){
    return M_PI/2.0 * cos( M_PI/2.0 * x );
};

double g ( double x ){
    return M_PI/4.0 * cos( M_PI/2.0 * x ) / ( 1 - x );
};
 
int main (int argc, char *argv[]){

    Random rnd;

    // Inizializzazione del generatore di numeri casuali: leggo i semi e i numeri primi dai rispettivi file
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
    ///////////////////////////////////////////////////////////////////////////

    int M = 100000;         //numero totale di lanci
    int N = 100;            //numero di blocchi
    int L = int(M/N);       //numero di elemento per blocco                                                                     

    //parte 1 - stima dell'integrale e della sua incertezza con una distribuzione uniforme tra 0 e 1

    ofstream output ("ese_02_1_pt1.out");

    double integral = 0.0;
    double integral2 = 0.0;

    for( int i = 0; i<N; i++ ){                  //ciclo for sui blocchi
        double sum = 0.0;

        for ( int j = 0; j < L; j++ ){            //ciclo for sugli elementi del blocco
            double x = rnd.Rannyu();
            sum += f(x);
        }

        integral += sum / double(L);
        integral2 += pow( sum / double(L), 2 );

        output << i+1 << " " << sum / double(L) << " " << integral/ double(i+1) << " " << error ( integral/ double(i+1), integral2/ double(i+1), i+1 ) << endl;
    }

    output.close();

    //parte 2 - stima dell'integrale e della sua incertezza con una distribuzione NON uniforme tra 0 e 1
    //importance sampling: p(x) = 2(1-x) => x = 1 - sqrt(1-rnd.Rannyu())

    ofstream output1 ("ese_02_1_pt2.out");

    integral = 0.0;
    integral2 = 0.0;

    for( int i = 0; i<N; i++ ){
        double sum = 0.0;

        for ( int j = 0; j < L; j++ ){
            double x = 1 - sqrt( 1 - rnd.Rannyu() );        //estraggo x secondo la distribuzione p(x)
            sum += g(x);                                    //in questo caso sommo g(x) = f(x)/p(x)
        }

        integral += sum / double(L);
        integral2 += pow( sum / double(L), 2 );

        output1 << i+1 << " " << sum / double(L) << " " << integral/ double(i+1) << " " << error ( integral/ double(i+1), integral2/ double(i+1), i+1 ) << endl;
      
    }

    output1.close();

    rnd.SaveSeed();          //per reiprendere la sequenza di semi dal punto in cui è terminata in caso di errori
    return 0;
}