#include "lib.h"
#include "popolazione.h"
#include <cmath>
#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

vector fitness; //vettore delle distanze

//CLASSE CROMOSOMA
//Costruttore, il cromosoma con le città in ordine corrisponde al primo individuo
Cromosoma :: Cromosoma (int n){
    this -> _n = n; //numero di geni
    this -> _cromosoma = vector<int> (_n); //dimensiona il vettore dei geni
    for (int i = 0; i<_n; i++){
        _cromosoma[i] = i+1; //cromosoma 0
    }
}

//Distruttore
Cromosoma :: ~Cromosoma (){
    _cromosoma.clear(); //libera la memoria allocata per il vettore dei geni
}

//Controlla se il cromosoma è composto da geni diversi
//ovvero passo da ogni città una sola volta
//e se la prima città visitata è la 1
bool Cromosoma :: check_passed (){
//parto sempre dalla prima città per ridurre la degenerazione
    if (_cromosoma[0] != 1){
            return false;
    }
    //controlla se il cromosoma è composto da geni diversi,
    // ovvero passo da ogni città una sola volta
    for (int i = 0; i<_n-1; i++){
        for (int j = i+1; j<_n; j++){
            if (_cromosoma[i] == _cromosoma[j]){
                return false;
            }
        }
    }
    return true;
}

//Calcola la fitness del cromosoma, ovvero la distanza tra le città che compaiono nel cromosoma
//La distanza tra le città vicine é calcolata con la matrice delle distanze
double Cromosoma :: set_fitness ( char geometry, Random &rnd){
    this -> _D = distanza_citta(_n, geometry, rnd); //distanza tra le città 
    double distance = 0.;
    for (int i = 0; i<_n-1; i++){
        int j = i+1;
        distance += _D(i,j);
    }
    distance += _D(_n-1, 0); //aggiungo la distanza tra l'ultima città e la prima
    return distance;
}

//Mutazione che scambia due città del cromosoma scelte in modo casuale
void Cromosoma :: mutazione_permutazione( Random &rnd){
    double r = rnd.Rannyu(2., (double)_n);
    double s = rnd.Rannyu(2., (double)_n);
    int i = 0;
    int j = 0;
    if ((r-(int)r)<0.5){
        i = (int)r;
    } else{
        i = (int)r+1;
    }
    if ((s-(int)s)<0.5){
        j = (int)s;
    } else{
        j = (int)s+1;
    }
    //scambio i e j
    int temp = _cromosoma[i];
    _cromosoma[i] = _cromosoma[j];
    _cromosoma[j] = temp;
    return;
}

//Stampa il cromosoma
void Cromosoma :: print_cromosoma (){
    ofstream out;
    out.open("cromosoma.dat");
    for (int i = 0; i<_n; i++){
        out << _cromosoma[i] << " ";
    }
    out << endl;
    out.close();
    return;
}


///////////////////////////////////////////////////////////////////
//CLASSE POPOLAZIONE
//Costruttore
Popolazione :: Popolazione(int _n, int _m, mat &D, Random &rnd){
    this -> _n = _n; //numero di geni
    this -> _m = _m; //numero di individui in una generazione
    //MODIFICA IN MODO CHE N E M SIANO LETTI DA FILE DI INPUT
    this -> _popolazione = vector<Cromosoma> (_m); //dimensiona il vettore della popolazione

}





/////////////////SISTEMA E CANCELLA
//Inizializza la prima genrerazione di m individui costituiti da n geni
void Popolazione :: initialize_population(int _n, int _m, char geometry, Random &rnd){

    popolazione.set_size(_m); // dimensiona il field a _m individui
    distances.set_size(_m); // dimensiona il vettore delle distanze a _m individui
    vec cromosoma(_n); //vettore dei geni
    
    //definisco il cromosoma zero con tutti gli interi in ordine da 1 a n
    for (int i = 0; i<_n; i++){
        cromosoma(i) = i+1; //cromosoma 0
    }
    popolazione(0) = cromosoma; 
    distances(0) = this -> get_distance(cromosoma, geometry); //calcolo la distanza del cromosoma 0

    //definisco gli altri cromosomi con permutazioni casuali
    for (int i = 1; i<_m; i++){
        this -> mutazione_permutazione(cromosoma, _n); //permutazione del cromosoma
        popolazione(i) = cromosoma; //cromosoma i 
        if (this -> check_passed(cromosoma, _n) == false){
            cout << "Cromosoma " << i << " non valido" << endl;
        }
        distances(i) = this -> get_distance(cromosoma, geometry); //calcolo la distanza del cromosoma i
    }
    return;
}


void Popolazione :: print_popolazione (){
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
}



