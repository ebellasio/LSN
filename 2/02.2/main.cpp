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

const double Pi = 2 * asin(1);

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

   int N_RW = 10000;         //numero totale di RW
   int N = 100;            //numero di blocchi in cui divido i RW
   int N_passi = 100;      //numero di passi per ogni RW
   int L = int(N_RW/N);       //numero di elemento per blocco                                                                 

   //part 1 - simulo un random walk in 3D partendo nell'origine su un reticolo cubuco di lato 1
   // 10000 volte ripeto il RW, ogni RW lo faccio per 100 passi e calcolo la distanza media
   // e la sua incertezza

   ofstream output ("ese_02_2_pt1.out");

   const double threshold = 1./6.; //soglia per il calcolo delle direzioni
   const double a = 1.0; //lattice constant
   double dist = 0.0;
   double dist2 = 0.0;

   vector<double> x(10000, 0.0);
   vector<double> y(10000, 0.0);
   vector<double> z(10000, 0.0);

   for ( int j = 0; j < N_passi; j++ ){

      dist = 0.0;
      dist2 = 0.0;

      for( int i = 0; i < N; i++ ){  //numero di RW per ogni blocco

         double sum = 0.0;

         for ( int m = 0; m < L; m++ ){      //numero di passi per ogni RW

            double direction = rnd.Rannyu();       //genero un numero tra 0 e 1 per definire la direzione
            if ( direction < threshold ){ x[m+i*L] += a; }                //direzione x
            else if ( threshold <= direction && direction < 2.0*threshold ){ x[m+i*L] -= a; } //direzione -x
            else if ( 2.0*threshold <= direction && direction < 3.0*threshold ){ y[m+i*L] += a; } //direzione y
            else if ( 3.0*threshold <= direction && direction < 4.0*threshold ){ y[m+i*L] -= a; } //direzione -y
            else if ( 4.0*threshold <= direction && direction < 5.0*threshold ){ z[m+i*L] += a; } //direzione z
            else { z[m+i*L] -= a; }                                       //direzione -z

            //cout << direction << " " << x[m+i*L] << " "<< y[m+i*L] << " " << z[m+i*L] << endl;
            sum += ( x[m+i*L]*x[m+i*L] + y[m+i*L]*y[m+i*L] + z[m+i*L]*z[m+i*L] );

         }

         dist += sqrt( sum / double(L) );
         dist2 += sum / double(L);

      }

      output << j+1 << " " << dist/ double(N) << " " << error ( dist/ double(N), dist2/ double(N), N ) << endl;
      
   }

   output.close();

   //part 2 - simulo un random walk in 3D partendo nell'origine nel continuo con passi discreti par ad a
   // 10000 volte ripeto il RW, ogni RW lo faccio per 100 passi e calcolo la distanza media
   // e la sua incertezza

   ofstream output2 ("ese_02_2_pt2.out");

   vector<double> x_sph(10000, 0.0);
   vector<double> y_sph(10000, 0.0);
   vector<double> z_sph(10000, 0.0);

   const double threshold_ang= 1./2.; //soglia per il calcolo del segno degli angoli theta e phi

   for ( int j = 0; j < N_passi; j++ ){

      dist = 0.0;
      dist2 = 0.0;  

      for( int i = 0; i < N; i++ ){  //numero di RW per ogni blocco

         double sum = 0.0;

         for ( int m = 0; m < L; m++ ){      //numero di passi per ogni RW

            double sign_theta = 1.0;
            double sign_phi = 1.0;

            double sin_theta = rnd.Rannyu();       //genero un numero tra 0 e 1 che rappresenta il seno dell'angolo theta tra 0 e pi
            if ( rnd.Rannyu() > threshold_ang ){ sign_theta = -1.0; } //se il numero è maggiore di 0.5 lo inverto
            double cos_theta = sign_theta * sqrt(1 - sin_theta*sin_theta); //coseno dell'angolo theta

            double sin_phi = rnd.Rannyu(-1, 1); //genero un numero tra -1 e 1 che rappresenta il seno dell'angolo phi tra 0 e 2pi
            if ( rnd.Rannyu() > threshold_ang ){ sign_phi = -1.0; } //se il numero è maggiore di 0 lo inverto
            double cos_phi = sign_phi * sqrt(1 - sin_phi*sin_phi); //coseno dell'angolo phi
            
            x_sph[m+i*L] += a * sin_theta * sin_phi; //direzione x
            y_sph[m+i*L] += a * sin_theta * cos_phi; //direzione y
            z_sph[m+i*L] += a * cos_theta;           //direzione z

            sum += ( x_sph[m+i*L]*x_sph[m+i*L] + y_sph[m+i*L]*y_sph[m+i*L] + z_sph[m+i*L]*z_sph[m+i*L] );

         }

         dist += sqrt( sum / double(L) );
         dist2 += sum / double(L);

      }

      output2 << j+1 << " " << dist/ double(N) << " " << error ( dist/ double(N), dist2/ double(N), N ) << endl;

   }

   output2.close();

   rnd.SaveSeed();//per reiprendere la stessa sequenza in caso di errori.
   return 0;
}