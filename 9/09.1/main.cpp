#include "lib.h"
#include "popolazione.h"

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

    int città = 34;
    int individui = 5;
    char geometry = 'C'; // C significa che le città sono disposte sulla circonferenza centrata in (0,0) di lato 1

    mat D = distanze_citta(città, geometry, rnd); // distanza tra le città poste sulla circonferenza
    //Costruisco la prima generazione di 100 cromosomi
    /*Popolazione P;
    P.initialize_population(città, individui, geometry, rnd); // inizializzo la popolazione con 100 cromosomi di 34 geni
    P.stampa(geni, individui);       */                        //
}