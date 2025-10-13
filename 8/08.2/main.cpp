/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include "lib.h"
#include "random.h"

using namespace std;
 
int main (){

    Random rnd;

    //Inizializzazione del generatore di numeri casuali: leggo i semi e i numeri primi dai rispettivi file
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
    double temp = 2.0; //temperatura iniziale
    Parametri p = {1.0, 0.5}; //parametri iniziali della funzione d'onda
    int n_steps = 800; //numero di step di simulated annealing
    double coeff_temp = 0.99; //coefficiente di raffreddamento

    ofstream out("output.dat");

    //Intertazione file di output
    out << "#STEP temp mu sigma <H> err" << endl;

    //800 iterazioni di simulated annealing
    for ( int i = 0; i < n_steps; i++){
        p = simulatedAnnealing(temp, p, rnd);
        temp *= coeff_temp;
        //stampo su file i risultati di ogni step
        out << i << " " << temp << " " << p.mu << " " << p.sigma << " " << H_avg(p, rnd).H_avg << " " << H_avg(p, rnd).H_err << endl;
    }

    out.close();

    rnd.SaveSeed();   //salvo il seme che serve per reiprendere la stessa sequenza in caso di errori
    return 0;
}