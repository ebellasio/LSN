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
#include <vector>

using namespace std;

//Funzione per il calcolo dell'errore statistico
double error( double av, double av2, int n ){
    if ( n == 0 ) return 0.0;
    else return sqrt( (av2 - av*av ) / double(n) );
};

//Funzione che ritorna il numero massimo tra due dati
double max( double a, double b ){
    if ( a > b ) return a;
    else return b;
};
 
int main (int argc, char *argv[]){

    Random rnd;
   
    ////////////////////////////////////////////////////////////////////////////per avere un numeo casuale fino a QUI
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
    ///////////////////////////////////////////////////////////////////////////////QUI

    double S_0 = 100.0;        //prezzo iniziale a t=0
    double T = 1.0;            //tempo di sviluppo
    double K = 100.0;          //prezzo di esercizio
    double r = 0.1;            //tasso di interesse
    double sigma = 0.25;       //volatilità

    //parte 1 - computo al tempo t=0 il prezzo della European call-option e della European put-option
    //con il metodo di Monte Carlo campionando direttammente il prezzo finale S(T) della call e della put

    //e calcolando il prezzo della call e della put come media dei payoff S(T) - K e K - S(T) rispettivamente
    //e dividendo per e^(r*T) il risultato

    ofstream outputP ("ese_03_1_pt1_P.out");
    ofstream outputC ("ese_03_1_pt1_C.out");

    int M = 10000;          //numero totale di lanci
    int N = 100;            //numero di blocchi
    int L = int(M/N);       //numero di elemento per blocco                                                                     

    double ave_P = 0.0;
    double ave2_P = 0.0;
    double ave_C = 0.0;
    double ave2_C = 0.0;

    for( int i = 0; i<N; i++ ){         //ciclo for sui blocchi
        double sum_P = 0.0;
        double sum_C = 0.0;
        double S_T = 0.0;

        for ( int j = 0; j < L; j++ ){  //ciclo for sugli elementi del blocco
            double Z = rnd.Gauss(0,1);
            S_T = S_0 * exp( (r - 0.5 * sigma * sigma) * T + sigma * sqrt(T) * Z );
            sum_C += exp(-r*T) * max(0.0, S_T - K);
            sum_P += exp(-r*T) * max(0.0, K - S_T);
        }

        ave_P += sum_P / double(L);
        ave2_P += pow( sum_P / double(L), 2 );
        ave_C += sum_C / double(L);
        ave2_C += pow( sum_C / double(L), 2 );

        outputP << i+1 << " " << sum_P / double(L) << " " << ave_P/ double(i+1) << " " << error ( ave_P/ double(i+1), ave2_P/ double(i+1), i+1 ) << endl;
        outputC << i+1 << " " << sum_C / double(L) << " " << ave_C/ double(i+1) << " " << error ( ave_C/ double(i+1), ave2_C/ double(i+1), i+1 ) << endl;
      
    }

    outputP.close();
    outputC.close();

    //parte 2 - computo al tempo t=0 il prezzo della European call-option e della European put-option
    //con il metodo di Monte Carlo campionando il percorso del prezzo di asset discretizzato dividento l'intervallo [0, T] in N=100 punti

    //e calcolando il prezzo della call e della put come media dei payoff S(T) - K e K - S(T) rispettivamente
    //e dividendo per e^(r*T) il risultato

    double N_T= 100;        //numero di punti in cui divido l'intervallo [0, T]

    ofstream output2P ("ese_03_1_pt2_P.out");
    ofstream output2C ("ese_03_1_pt2_C.out");

    ave_P = 0.0;
    ave2_P = 0.0;
    ave_C = 0.0;
    ave2_C = 0.0;

    for( int i = 0; i<N; i++ ){                  //ciclo for sui blocchi
        double sum_P = 0.0;   
        double sum_C = 0.0;
        double S_T = 0.0;

        for ( int j = 0; j < L; j++ ){           //ciclo for sugli elementi del blocco

            double S_ti = S_0;

            for ( int k = 1; k <= N_T; k++ ){    //ciclo for sui punti in cui divido l'intervallo [0, T]
                double Z = rnd.Gauss(0,1);
                S_T = S_ti * exp( (r - 0.5 * sigma * sigma) * (T/N_T) + sigma * sqrt(T/N_T) * Z );
                S_ti = S_T;
            }

            sum_C += exp(-r*T) * max(0.0, S_T - K);
            sum_P += exp(-r*T) * max(0.0, K - S_T);
        }

        ave_P += sum_P / double(L);
        ave2_P += pow( sum_P / double(L), 2 );
        ave_C += sum_C / double(L);
        ave2_C += pow( sum_C / double(L), 2 );

        output2P << i+1 << " " << sum_P / double(L) << " " << ave_P/ double(i+1) << " " << error ( ave_P/ double(i+1), ave2_P/ double(i+1), i+1 ) << endl;
        output2C << i+1 << " " << sum_C / double(L) << " " << ave_C/ double(i+1) << " " << error ( ave_C/ double(i+1), ave2_C/ double(i+1), i+1 ) << endl;
      
    }

    output2P.close();
    output2C.close();

    rnd.SaveSeed();    //per reiprendere la stessa sequenza in caso di errori.
    return 0;
}