#include "lib.h"
#include "popolazione.h"
#include <cmath>
#include <iostream>
#include <armadillo>
#include <fstream>

using namespace std;
using namespace arma;

vector fitness; //vettore delle distanze

//CLASSE CROMOSOMA
//Costruttore -> il primo cromosoma ha le città in ordine crescente
Cromosoma :: Cromosoma (int n){
    this -> _n = n; //numero di geni
    this -> _cromosoma.resize(_n); //dimensiona il vettore cromosoma
    for (int i = 0; i < _n; i++){
        _cromosoma[i] = i+1; //cromosoma 0
    }
    if (this->check_passed() == false){
        cout << "Errore: non sono state rispettare le condizioni sull'ordine delle città" << endl;
        return;
    }
}

//Distruttore
Cromosoma :: ~Cromosoma (){
    _cromosoma.clear(); //libera la memoria allocata per il vettore dei geni
}

//Controlla se il cromosoma è composto da geni diversi, ovvero passo da ogni città una sola volta
//e se la prima città visitata è la 1
bool Cromosoma :: check_passed (){
    if (_cromosoma[0] != 1){
            return false; //la prima città è sempre la 1 per ridurre la degenerazione
    }
    for (int i = 0; i < _n-1; i++){
        for (int j = i+1; j < _n; j++){
            if (this->_cromosoma[i] == this->_cromosoma[j]){
                return false; //devo passare una sola volta da ciascuna città
            }
        }
    }
    return true; //se arrivo qui le condizioni sono rispettate
}

//Calcola la fitness del cromosoma, ovvero la distanza tra le città che compaiono nel cromosoma
//La distanza tra le città vicine è calcolata con la matrice delle distanze
void Cromosoma :: fitness (){
    this->_fitness = 0;
    for (int i = 0; i < _n-1; i++){
        int j = i+1;
        this->_fitness += this->_D(i,j);
    }
    this->_fitness += this->_D(_n-1, 0); //aggungo la distanza tra l'ultima città e la prima
}

//Stampa il cromosoma
void Cromosoma :: print_cromosoma (ofstream &out){
    for (int i = 0; i < this->_n; i++){
            out << this->_cromosoma[i] << " ";
    }
}

//stampa la fitness
void Cromosoma :: print_fitness (ofstream &out){
    out << this->_fitness << endl;
}

//Mutazione che scambia due città del cromosoma scelte in modo casuale
//La utilizzo anche per generare la popolazone di partenza
void Cromosoma :: Permutazione( Random &rnd){
    int i = (int)rnd.Rannyu(1, _n); // indice tra 1 e _n-1 (escluso 0 e quindi la prima città)
    int j = (int)rnd.Rannyu(1, _n);
    while (j == i) { // assicura che i e j siano diversi
        j = (int)rnd.Rannyu(1, _n);
    }
    //scambio i e j
    int temp = this->_cromosoma[i];
    this->_cromosoma[i] = this->_cromosoma[j];
    this->_cromosoma[j] = temp;
    if (this->check_passed() == false){
        cout << "Errore dopo la mutazione (Permutazione)" << endl;
        return;
    }
}

//Mutazione che sposta di n posizioni m città consecutive
void Cromosoma :: Shift (Random &rnd){
    int l = rnd.Rannyu(1, _n-2); //prima città da spostare
    int m = rnd.Rannyu(2, _n-l); //dimensione del blocco da spostare
    int j = rnd.Rannyu(1, _n-l-m+1); //di quanto spostare il blocco
    for ( int i = 0; i < m; i++ ){
        int temp = this->_cromosoma[l+1];
        this->_cromosoma[l+1] = this->_cromosoma[l+j+1];
        this->_cromosoma[l+j+i] = temp;
    }
    if (this->check_passed() == false){ //controllo se il cromosoma rispetta i vincoli
        cout << "Errore dopo la mutazione (Shift)" << endl;
        return;
    }
}

//Mutazione che scambia due blocchi di m città consecutive
void Cromosoma :: Scambio (Random &rnd){
    int max_m = (_n - 1) / 2; //Lunghezza massima del blocco
    int m = rnd.Rannyu(1, max_m + 1); // m in [1, max_m]
    int i = rnd.Rannyu(1, _n - 2 * m + 1); // posizione del primo blocco, i in [1, _n-2m]
    int j = rnd.Rannyu(i + m, _n - m + 1); // posizione del secondo blocco, j in [i+m, _n-m]
    for (int k = 0; k < m; ++k) {
        int temp = this->_cromosoma[i+k];
        this->_cromosoma[i+k] = this->_cromosoma[j+k];
        this->_cromosoma[j+k] = temp;
    }
    if (this->check_passed() == false) {
        cout << "Errore dopo la mutazione (Scambio blocchi)" << endl;
        return;
    }
}

//Mutazione che inverte l'ordine in cui compaiono m città in un individuo
void Cromosoma :: Inversione (Random &rnd){
    int i = rnd.Rannyu(1, _n-2); //posizione del primo elemento da invertire
    int j = rnd.Rannyu(2, _n-i); //lunghezza del blocco da invertire
    for ( int n = 0; n < j/2; n++){
        int m = j-1;
        int temp = this->_cromosoma[m+i];
        this->_cromosoma[m+i] = this->_cromosoma[n+i];
        this->_cromosoma[n+i] = temp;
        j--;
    }
    if (this->check_passed() == false) {
        cout << "Errore dopo la mutazione (Inversione)" << endl;
        return;
    }
}

///////////////////////////////////////////////////////////////////
//CLASSE POPOLAZIONE
//Costruttore
Popolazione :: Popolazione( int n, int m, double p_m, double p_c, mat D, Random &rnd){
    this->_n = n; //numero di città
    this->_m = m; //numero di individui in ciascuna generazione
    this->_p_m = p_m; //probabilità di mutazione
    this->_p_c = p_c; //probabilità di crossover

    this->_popolazione = vector<Cromosoma> (m, Cromosoma(this->_n)); //dimensiona il vettore della popolazione
    for ( int i = 0; i < m; i++ ){
        this->_popolazione[i].Set_distances(D); //associo a ciascun individuo la matrice delle distanze
        for (int j = 0; j < m; j++ ){
            this->_popolazione[i].Permutazione(rnd); //a partire dall'individuo iniziale creo tutti gli altri con una parmutazione
        }
        this->_popolazione[i].fitness(); //a ciascun individuo associo la fitness
    }
}






///RIPARTO DA QUI
/*void Popolazione :: print_popolazione (){
    ofstream out;
    out.open("popolazione.dat");
    for (int i = 0; i<_m; i++){
        for (int j = 0; j<_n; j++){     //j<popolazione(i).n_elem
            out << (int)_popolazione[i][j] << " ";
        }
        out << endl;
    }
    out.close();
    return;
}*/



