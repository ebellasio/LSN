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

// funzione che calcola l'errore statistico (non utilizzata in questo esercizio)
double error( double av, double av2, int n ){
    if ( n == 0 ) return 0.0;
    else return sqrt( (av2 - av*av ) / double(n) );
}

// funzione dado che restuisce un numero intero casuale tra 1 e 6 (uniforme)
int dice( Random &rnd ){
    return ( (rnd.Rannyu() * 6 ) /1 ) + 1;
}

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
    ///////////////////////////////////////////////////////////////

    int M = 10000;                           //numero totale di ripetizioni di S_N
    int N[4] = {1, 2, 10, 100};              //vettore che contiene i valori di N = 1, 2, 10, 100.                         

    const double lambda = 1.0;                //parametro della distribuzione esponenziale
    const double gamma = 1.0;                 //parametro della distribuzione di Cauchy-Lorentz
    const double mu = 0.0;                    //parametro della distribuzione di Cauchy-Lorentz

    ofstream output1 ("ese_01_2_uniform.out");
    ofstream output2 ("ese_01_2_exponential.out");
    ofstream output3 ("ese_01_2_cauchy_lorentz.out");

    for( int i = 0; i < M; i++ ){

        double avg_unif[4] = {0.0};    //vettore che contiene la media S_N (per i vari N) per la distribuzione uniforme
        double avg_exp[4] = {0.0};     //vettore che contiene la media S_N (per i vari N) per la distribuzione esponenziale
        double avg_lorentz[4] = {0.0}; //vettore che contiene la media S_N (per i vari N) per la distribuzione di Cauchy-Lorentz

        for ( int j = 0; j < 4; j++ ){             //ciclo sui diversi valori di N
            
            for ( int k = 0; k < N[j]; k++ ){      
                avg_unif[j] += dice( rnd );                       //distribuzione uniforme definita sopra
                avg_exp[j] += rnd.Exponential( lambda );          //distribuzione esponenziale definita in random.cpp
                avg_lorentz[j] += rnd.CauchyLorentz( mu, gamma ); //distribuzione di Cauchy-Lorentz definita in random.cpp
            }

            avg_unif[j] /= double(N[j]);             //calcolo la media dei valori estratti
            avg_exp[j] /= double(N[j]);
            avg_lorentz[j] /= double(N[j]);
        }

        output1 << i+1 << " " << avg_unif[0] << " " << avg_unif[1] << " " << avg_unif[2] << " " << avg_unif[3] << endl;
        output2 << i+1 << " " << avg_exp[0] << " " << avg_exp[1] << " " << avg_exp[2] << " " << avg_exp[3] << endl;
        output3 << i+1 << " " << avg_lorentz[0] << " " << avg_lorentz[1] << " " << avg_lorentz[2] << " " << avg_lorentz[3] << endl;

    }

   output1.close();
   output2.close();
   output3.close();

   rnd.SaveSeed(); //salvo il seme per poter riprendere la generazione di numeri casuali dal punto corrente
   return 0;
}