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

// funzione per calcolare l'incertezza statistica
double error( double av, double av2, int n ){
    if ( n == 0 ) return 0.0;
    else return sqrt( (av2 - av*av ) / double(n) );
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
    //////////////////////////////////////////////////////////////////////////////



    //parte 1 - calcolo <r> e la sua incertezza

    int M = 100000;         //numero totale di lanci
    int N = 100;            //numero di blocchi
    int L = int(M/N);       //numero di elemento per blocco                                                                     

    ofstream output ("ese_01_1_pt1.out");

    double ave = 0.0;
    double ave2 = 0.0;

    for( int i = 0; i<N; i++ ){   //ciclo for sui blocchi
        double sum = 0.0;         //metto a zero la somma dei lanci prima di sommare gli elementi di un nuovo blocco

        for ( int j = 0; j < L; j++ ){    //ciclo for sugli elementi del blocco
            sum += rnd.Rannyu();          //sommo l'argomento dell'integrale: i lanci del blocco
        }

        ave += sum / double(L);           //aggiungo agli accumulatori la media/quadrato della media di ogni blocco
        ave2 += pow( sum / double(L), 2 );

        output << i+1 << " " << sum / double(L) << " " << ave/ double(i+1) << " " << error ( ave/ double(i+1), ave2/ double(i+1), i+1 ) << endl;
      
    }

    output.close();

    //parte 2 - calcolo sigma^2 e la sua incertezza

    ofstream output2 ("ese_01_1_pt2.out");

    ave = 0.0;
    ave2 = 0.0;

    for( int i=0; i<N; i++ ){
        double sum = 0.0;

        for ( int j = 0; j < L; j++ ){
            sum += pow ((rnd.Rannyu() - 0.5), 2);   //sommo il qadrato dei lanci meno il risultato atteso (argomento dell'integale)
        }

        ave += sum / double(L);
        ave2 += pow( sum / double(L), 2 );

        output2 << i+1 << " " << sum / double(L) << " " << ave/ double(i+1) << " " << error ( ave/ double(i+1), ave2/ double(i+1), i+1 ) << endl;
      
    }

    output2.close();

    //parte 3 - test del chi quadro

    ofstream output3 ("ese_01_1_pt3.out");

    M = 100;          //sottointervalli (bins) identici in cui dividere l'intervallo [0,1]
    int n = 10000;    //numero di lanci

    for( int i = 0; i < M; i++ ){
        vector <int> conteggi_bins (M,0); //vettore per salvare i conteggi in ogni bin
        double chi2 = 0.0;                //variabile per calcolare il chi quadro

        for ( int j = 0; j < n; j++ ){
            double r = rnd.Rannyu();      //estraggo un numero casuale
            conteggi_bins[floor(r*M)] ++; //incremento il conteggio del bin in cui cade il numero estratto
        }

        for ( int k = 0; k < M; k++ ){
            chi2 += pow( ((double)conteggi_bins[k] - double(n)/double(M)), 2 ) / (double(n)/double(M));  //calcolo il chi quadro totale
        }

        output3 << i+1 << " " << chi2 << endl;
    }

    output3.close();

    rnd.SaveSeed();   //salvo il seme che serve per reiprendere la stessa sequenza in caso di errori
    return 0;
}