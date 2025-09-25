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

double error( double av, double av2, int n ){
   if ( n == 0 ) return 0.0;
   else return sqrt( (av2 - av*av ) / double(n) );
};

// funzione dado che restuisce un numero casuale tra 1 e 6
int dice( Random &rnd ){
   return ( (rnd.Rannyu() * 6 ) /1 ) + 1;
}

// Funzione che simula l'esperimento di Buffon
double Buffon( Random &rnd, double d, double nl ){
   //rnd: Generatore di N casuali
   //N_th: Numero di lanci
   //d: Distanza tra le righe in cm
   //nl: Lunghezza dell'ago in cm

   //Distanza del centro dell'ago dalla riga più vicina
   double dist = rnd.Rannyu(0, d/2);
   //Angolo tra l'ago e la linea
   double theta = rnd.Rannyu(0, Pi/2);    //angolo tra 0 e Pi/2 perchè oltre Pi/2 ho simmetria
   //Proiezione dell'ago ortogonale alle linee
   double proj = (nl / 2) * sin(theta);
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

   int M = 100000;         //numero totale di lanci
   int N = 100;            //numero di blocchi
   int L = int(M/N);       //numero di elemento per blocco      
   double const nl = 4.;       //lunghezza dell'ago in cm
   double const d = 4.5;       //distanza tra le righe in cm

   ofstream output ("ese_01_3.out");

   double P = 0.0;
   double pi = 0.0;
   double pi2 = 0.0;
   

   for( int i = 0; i<N; i++ ){
      double sum = 0.0;

      for ( int j = 0; j < L; j++ ){
         sum += Buffon( rnd, d, nl );
      }

      P += sum / double(L);
      double pr = P / double(i+1);
      pi += ( 2 * nl ) / ( pr * d );
      pi2 += pow( ( ( 2 * nl ) / ( pr * d ) ), 2 );

      output << i+1 << " " << P / double (i+1) << " " << pi / double (i+1) << " " << error ( pi/ double(i+1), pi2/ double(i+1), i+1 ) << endl;
      
   }

   output.close();
  
   rnd.SaveSeed();//per reiprendere la stessa sequenza in caso di errori.
   return 0;
}