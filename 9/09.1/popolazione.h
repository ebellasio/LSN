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
        int _n; //numero di geni/città
        vector<int> _cromosoma; //vettore che contine i geni, ovvero le città nell'ordine in cui vengono visitate
        double _fitness; //fitness del percorso, ovvero distanza tra le città
        mat _D; //matrice delle distanze tra le città
    public:
        Cromosoma (int _n); //costruttore
        ~Cromosoma (); //distruttore
        bool check_passed (); //controlla se il cromosoma è composto da geni diversi e se la prima città visitata è la 1
        void fitness (); //calcola la distanza tra le città che compaiono nel cromosoma
        void Set_distances(mat dist) {this->_D = dist;}; //setta le distanze tra le città

        void print_cromosoma (ofstream &out); //stampa il cromosoma
        void print_fitness (ofstream &out); //stampa la fitness

        void Permutazione(Random &rnd);  //scambia due città del cromosoma
        void Shift (Random &rnd); //sposta di n posizioni m città consecutive
        void Scambio (Random &rnd); // scambia due blocchi di m città consecutive
        void Inversione (Random &rnd); //inverto la posizione di m città consecutive

};

//Classe popolazione
class Popolazione {
    private:
        int _n; //numero di città
        int _m; //numero di individui/cromosomi in una generazione
        int _index_gen = 0; //indice per le generazioni
        vector<Cromosoma> _popolazione; //matrice della popolazione (insieme di cromosomi)

        double _p_m; //probabilità mutazione
        double _p_c; //probabilità crossover

    public:
        Popolazione(int n, int m, double p_m, double p_c, mat D, Random &rnd);
        //void print_popolazione ();
        //double get_distance ( vec cromosoma, char geometry );
};