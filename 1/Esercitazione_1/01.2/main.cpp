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
// funzione dado che restuisce un numero casuale tra 1 e 6
int dice( Random &rnd ){
   return ( (rnd.Rannyu() * 6 ) /1 ) + 1;
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

   //part 1 - estimation of the mean value and its uncertaionty

   int M = 10000;                   //numero totale di lanci
   int vec[4] = {1, 2, 10, 100};   //vettore che contiene 1, 2, 10, 100.  
   double avg[4] = {0.0, 0.0, 0.0, 0.0};   //vettore che contiene la media dei valori estratti                       

   ofstream output1 ("ese_01_2_uniform.out");
   ofstream output2 ("ese_01_2_exponential.out");
   ofstream output3 ("ese_01_2_cauchy_lorentz.out");

   const double lambda = 1.0;
   const double gamma = 1.0;
   const double mu = 0.0;

   for( int i = 0; i<M; i++ ){

      //uniform distribution - standard dice

      for ( int j = 0; j < 4; j++ ){
         double sum = 0.0;

         for ( int k = 0; k < vec[j]; k++ ){
            sum += dice( rnd );
         }
         
         avg[j] = sum / double(vec[j]);
      }

      output1 << i+1 << " " << avg[0] << " " << avg[1] << " " << avg[2] << " " << avg[3] << endl;

      //exponential distribution
      for ( int j = 0; j < 4; j++ ){
         double sum = 0.0;

         for ( int k = 0; k < vec[j]; k++ ){
            sum += rnd.Exponential( lambda );
         }
         
         avg[j] = sum / double(vec[j]);
      }

      output2 << i+1 << " " << avg[0] << " " << avg[1] << " " << avg[2] << " " << avg[3] << endl;

      //Cauchy Lorentz distribution
      for ( int j = 0; j < 4; j++ ){
         double sum = 0.0;

         for ( int k = 0; k < vec[j]; k++ ){
            sum += rnd.CauchyLorentz( mu, gamma );
         }
         
         avg[j] = sum / double(vec[j]);
      }

      output3 << i+1 << " " << avg[0] << " " << avg[1] << " " << avg[2] << " " << avg[3] << endl;
      
      
   }

   output1.close();
   output2.close();
   output3.close();

   rnd.SaveSeed();//per reiprendere la stessa sequenza in caso di errori.
   return 0;
}