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

const double Pi = 2 * asin(1);

// funzione che calcola l'errore statistico
double error( double av, double av2, int n ){
    if ( n == 0 ) return 0.0;
    else return sqrt( (av2 - av*av ) / double(n) );
};

// funzione per calcolare pi greco per assumere di non conoscerlo a priori
// considero un quadrato di lato 2 [-1,1] e la circonferenza inscritta
// il rapporto tra i punti che cadono all'interno della circonferenza e il totale
// dei punti nel cerchio tende a pi/4 per un numero di punti che tende a infinito
double pi_greco( Random &rnd, int N_th ){
    //rnd: Generatore di N casuali
    //N_th: Numero di lanci
    int count = 0; //contatore dei punti che cadono all'interno della circonferenza

    for ( int i = 0; i < N_th; i++ ){
        double x = rnd.Rannyu(-1.0,1.0);         //estraggo la coordinata x
        double y = rnd.Rannyu(-1.0,1.0);         //estraggo la coordinata y

        if ( x*x + y*y <= 1 ) count++;           //controllo se il punto cade all'interno della circonferenza
    }

    return 4.0 * double(count) / double(N_th); //restituisco il valore di pi greco calcolato
}


// Funzione che simula l'esperimento di Buffon
double Buffon( Random &rnd, double d, double nl, double Pi ){
    //rnd: Generatore di N casuali
    //d: Distanza tra le righe in cm
    //nl: Lunghezza dell'ago in cm

    //Distanza del centro dell'ago dalla riga più vicina
    double dist = rnd.Rannyu(0.0, d/2);
    //Angolo tra l'ago e la linea
    double theta = rnd.Rannyu(0.0, Pi/2);    //angolo tra 0 e Pi/2 perchè oltre Pi/2 ho simmetria
    //Proiezione dell'ago ortogonale alle linee
    double proj = (nl / 2.0) * sin(theta);
    //Controllo se l'ago interseca la linea
    if ( proj > dist ) return 1;
    else return 0;
}

int main (int argc, char *argv[]){

    Random rnd;
   
    ////////////////////////////////////////////////////////////////////////////per avere un numeo casuale fino a QUI
    int seed[4];                                                                  
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    }else cerr << "PROBLEM: Unable to open Primes" << endl;
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

    int M = 100000;             //numero totale di lanci
    int N = 100;                //numero di blocchi
    int L = int(M/N);           //numero di elemento per blocco      

    double const nl = 3.;       //lunghezza dell'ago in cm
    double const d = 3.6;       //distanza tra le righe in cm

    double pi_exp = 0.0;        //valore di pi greco calcolato con l'esperimento di Buffon
    double pi2_exp = 0.0;       //valore di pi greco al quadrato calcolato con l'esperimento di Buffon

    double pi_valore = pi_greco( rnd, 100000000 ); //valore di pi greco calcolato 
    cout << "Valore di pi greco calcolato: " << pi_valore << endl;
   
    ofstream output ("ese_01_3.out");

    for( int i = 0; i<N; i++ ){
        double sum = 0.0;

        for ( int j = 0; j < L; j++ ){
            sum += Buffon( rnd, d, nl, pi_valore );
        }

        pi_exp += ( 2 * nl * L) / ( sum * d );               //L è il numero di lanci effettuati in questo blocco
        pi2_exp += pow( (( 2 * nl * L) / ( sum * d )), 2 );  //sum rappresenta il numero di volte che l'ago ha intersecato una riga

        output << i+1 << " " << ( 2 * nl * L) / ( sum * d ) << " " << pi_exp / double (i+1) << " " << error ( pi_exp/ double(i+1), pi2_exp/ double(i+1), i+1 ) << endl;
      
   }

   output.close();
  
   rnd.SaveSeed();//per reiprendere la stessa sequenza in caso di errori.
   return 0;
}