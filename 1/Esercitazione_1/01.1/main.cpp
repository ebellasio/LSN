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

   //part 1 - estimation of the mean value and its uncertaionty

   int M = 100000;         //numero totale di lanci
   int N = 100;            //numero di blocchi
   int L = int(M/N);       //numero di elemento per blocco                                                                     

   ofstream output ("ese_01_1_pt1.out");

   double ave = 0.0;
   double ave2 = 0.0;

   for( int i = 0; i<N; i++ ){
      double sum = 0.0;

      for ( int j = 0; j < L; j++ ){
         sum += rnd.Rannyu();
      }

      ave += sum / double(L);
      ave2 += pow( sum / double(L), 2 );

      output << i+1 << " " << sum / double(L) << " " << ave/ double(i+1) << " " << error ( ave/ double(i+1), ave2/ double(i+1), i+1 ) << endl;
      
   }

   output.close();

   //part 2 - estimation of sigma2 and its uncertaionty

   ofstream output2 ("ese_01_1_pt2.out");

   ave = 0.0;
   ave2 = 0.0;

   for( int i = 0; i<N; i++ ){
      double sum = 0.0;

      for ( int j = 0; j < L; j++ ){
         sum += pow ((rnd.Rannyu() - 0.5), 2);
      }

      ave += sum / double(L);
      ave2 += pow( sum / double(L), 2 );

      output2 << i+1 << " " << sum / double(L) << " " << ave/ double(i+1) << " " << error ( ave/ double(i+1), ave2/ double(i+1), i+1 ) << endl;
      
   }

   output2.close();

   //part 3 - chi2 test

   ofstream output3 ("ese_01_1_pt3.out");

   double expected = 1.0 / 2.0;

   for( int i = 0; i<N; i++ ){

      double chi2 = 0.0;

      for ( int j = 0; j < L; j++ ){
         chi2 += pow ((rnd.Rannyu() - expected), 2);
      }

      output3 << i+1 << " " << chi2 / expected << endl;
      
   }

   output3.close();

   rnd.SaveSeed();//per reiprendere la stessa sequenza in caso di errori.
   return 0;
}