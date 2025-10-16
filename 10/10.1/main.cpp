#include "lib.h"
#include "popolazione.h"
#include <iostream>
#include <fstream>
#include <armadillo>

using namespace std;

int main(int argc, char *argv[]) {

    Random rnd;

    // Inizializzazione del generatore di numeri casuali: leggo i semi e i numeri primi dai rispettivi file
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open())
    {
        Primes >> p1 >> p2;
    }
    else
        cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open())
    {
        while (!input.eof())
        {
            input >> property;
            if (property == "RANDOMSEED")
            {
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed, p1, p2);
            }
        }
        input.close();
    }
    else
        cerr << "PROBLEM: Unable to open seed.in" << endl;
    ///////////////////////////////////////////////////////////////

    int geni = 34; //numero di città
    int individui = 200; //numero di individui/cromosomi in una generazione
    double p_m = 0.08; //probabilità di mutazione
    double p_c = 0.7; //probabilità di crossover
    int n_generazioni = 500; //numero di generazioni

    //CIRCONFERENZA
    char geometry = 'C'; // C significa che le città sono disposte sulla circonferenza centrata in (0,0) di lato 1

    mat dist_C = distanze_citta(geni, geometry, rnd); // distanza tra le città poste sulla circonferenza
    Popolazione Pop_C(geni, individui, p_m, p_c, dist_C, rnd); //creo la popolazione iniziale
    Pop_C.Sort(); //ordino la popolazione in ordine crescente di fitness

    ofstream out_pop ("./OUTPUT/popolazione_C.dat"); //output per la popolazione
    if (!out_pop.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    ofstream out_best ("./OUTPUT/migliore_C.dat"); //output per il migliore individuo
    if (!out_best.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    out_pop << "#" << " " << "Generazione:" << " " << "Individuo:" << " " << "Fitness:" << endl; //intestazione del file popolazione.dat
    Pop_C.print_popolazione(out_pop); //stampo la popolazione iniziale
    
    for ( int i=0; i<n_generazioni; i++ ){
        Pop_C = Pop_C.NewGenerationCrossover(Pop_C, rnd); //creo la nuova generazione
        Pop_C.print_popolazione(out_pop); //stampo la nuova generazione
    }
    
    Pop_C.GetBest().print_cromosoma(out_best); //stampo il migliore individuo per ciascuna generazione

    out_best.close();
    out_pop.close();

    //QUADRATO
    geometry = 'Q'; // Q significa che le città sono disposte su un quadrato di lato 1

    mat dist_Q = distanze_citta(geni, geometry, rnd); // distanza tra le città poste sul quadrato
    Popolazione Pop_Q(geni, individui, p_m, p_c, dist_Q, rnd); //creo la popolazione iniziale
    Pop_Q.Sort(); //ordino la popolazione in ordine crescente di fitness

    ofstream out_pop_Q ("./OUTPUT/popolazione_Q.dat"); //output per la popolazione
    if (!out_pop_Q.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    ofstream out_best_Q ("./OUTPUT/migliore_Q.dat"); //output per il migliore individuo
    if (!out_best_Q.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    out_pop_Q << "#" << " " << "Generazione:" << " " << "Individuo:" << " " << "Fitness:" << endl; //intestazione del file popolazione.dat
    Pop_Q.print_popolazione(out_pop_Q); //stampo la popolazione iniziale
    
    for ( int i=0; i<n_generazioni; i++ ){
        Pop_Q =Pop_Q.NewGenerationCrossover(Pop_Q, rnd); //creo la nuova generazione
        Pop_Q.print_popolazione(out_pop_Q); //stampo la nuova generazione
    }

    Pop_Q.GetBest().print_cromosoma(out_best_Q); //stampo il migliore individuo per ciascuna generazione

    out_best_Q.close();
    out_pop_Q.close();

    rnd.SaveSeed();
}