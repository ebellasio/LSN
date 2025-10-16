#pragma once

#include "random.h"
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit

using namespace std;
using namespace arma;

// Funzione che calcola la matrice delle distanze tra le città
mat distanze_citta ( int n, char S, Random &rnd, int rank);

//Classe cromosoma -> rappresenta un individuo della popolazione, insieme di _n geni
class Cromosoma {
    private:
        int _n; //numero di geni/città
        vector<int> _cromosoma; //vettore che contine i geni, ovvero le città nell'ordine in cui vengono visitate
        double _fitness; //fitness del percorso, ovvero distanza tra le città
        mat _D; //matrice delle distanze tra le città
    public:
        Cromosoma (int _n); //costruttore
        /*Cromosoma (const Cromosoma& other); //costruttore di copia
        Cromosoma& operator=(const Cromosoma& other); //operatore di assegnazione */

        bool check_passed (); //controlla se il cromosoma è composto da geni diversi e se la prima città visitata è la 1
        void fitness (); //calcola la distanza tra le città che compaiono nel cromosoma

        bool operator<(const Cromosoma& other) const { //definisco l'operatore < per ordinare i cromosomi in base alla fitness
            return _fitness < other._fitness;
        }

        void Set_distances(mat dist) {this->_D = dist;}; //setta le distanze tra le città

        int GetGene (int i) {
            if (i < 0 || i >= this->_n) {
                cout << "Errore: indice fuori dai limiti" << endl;
                return -1;
            }
            return this->_cromosoma[i];
        };

        void SetGene (int i, int val) {
            if (i < 0 || i >= this->_n) {
                cout << "Errore: indice fuori dai limiti" << endl;
                return;
            }
            this->_cromosoma[i] = val;
        };

        double GetFitness () {return this->_fitness;};
        int GetN () const {return this->_n;};

        void print_cromosoma (ofstream &out); //stampa il cromosoma
        void print_fitness (ofstream &out); //stampa la fitness

        void Permutazione(Random &rnd);  //scambia due città del cromosoma
        void Shift (Random &rnd); //sposta di n posizioni m città consecutive
        void Scambio (Random &rnd); // scambia due blocchi di m città consecutive
        void Inversione (Random &rnd); //inverto la posizione di m città consecutive

        vector<int> Serialize(){ //restituisce il cromosoma come vettore di int
            vector<int> data = _cromosoma;
            return data;    
        }

        void Deserialize(const vector<int>& data){ //setta il cromosoma a partire da un vettore di int
            _cromosoma = data;
        }

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

        int GetN() {return this->_n;};
        int GetGen() {return this->_index_gen;};
        void NextGen() {this->_index_gen += 1;};

        void print_popolazione (ofstream &out);
        void Sort();

        Cromosoma GetBest() {return this->_popolazione[0];};
        void SetBest(const Cromosoma& c) {this->_popolazione[0] = c;};

        int Select(Random &rnd) const; //seleziona un individuo dalla popolazione 

        Popolazione NewGenerationCrossover( Popolazione pop_old, Random &rnd); //crea la nuova generazione

};

void migrazione (Popolazione &pop, int rank, int size, mat D, Random &rnd);


