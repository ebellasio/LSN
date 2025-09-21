#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit
#include "random.h"
#include "lib.h"

using namespace std;
using namespace arma;

//Classe cromosoma -> rappresenta un individuo della popolazione, insieme di _n geni
class Cromosoma {
    private:
        int _n; //numero di geni
        vector<int> _cromosoma; //vettore che contine i geni, ovvero le città nell'ordine in cui vengono visitate
        double _fitness; //fitness del percorso
        mat _D; //matrice delle distanze tra le città
    public:
        Cromosoma (int _n); //costruttore
        ~Cromosoma (); //distruttore

        bool check_passed (); //controlla se il cromosoma è composto da geni diversi e se la prima città visitata è la 1
        
        double set_fitness ( char geometry, Random &rnd ); //calcola la distanza tra le città che compaiono nel cromosoma
        double get_fitness ();

        void mutazione_permutazione( Random &rnd);  //scambia due città del cromosoma

        void print_cromosoma (); //stampa il cromosoma

};

//Classe popolazione
class Popolazione {
    private:
        int _n; //numero di città
        int _m; //numero di individui in una generazione
        vector<Cromosoma> _popolazione; //matrice della popolazione (insieme di cromosomi)

    public:
        Popolazione(int _n, int _m, mat &D, Random &rnd);
        void print_popolazione ();
        double get_distance ( vec cromosoma, char geometry );
};