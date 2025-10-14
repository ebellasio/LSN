#include "lib.h"
#include "random.h"
#include <cmath>
#include <iostream>
#include <armadillo>
#include <fstream>


using namespace std;
using namespace arma;

// Funzione che calcola la matrice delle distanze tra le città
// Restituisce una matrice di dimensione n x n, dove n è il numero di città
// La matrice contiene le distanze euclidee tra le città 
mat distanze_citta ( int n, char schema, Random &rnd){

    mat distanze (n, n); //matrice che contiene le distanze tra due città 
    mat coordinate (n, 2); //matrice che contiene le coordinate (x, y) di ogni città
   
    // C significa che le città sono disposte sulla circonferenza centrata in (0,0) di lato 1
    if (schema == 'C'){
        ofstream out ("./OUTPUT/citta_C.dat"); //output per le città sul cerchio
        if (!out.is_open()){
            cout << "Error opening file" << endl;
        }
        out << "#" << " " << "X:" << " " << "Y:" << endl; //intestazione del file

        vector<double> ang(n, 0.0); //vettore di angoli di ogni città inizializzato a zero
        for ( int i = 1; i < n; i++ ){ //parto da uno perchè assegno alla prima città l'angolo zero
            ang[i] = rnd.Rannyu(0., 2 * M_PI);
        }
        sort(ang.begin(), ang.end()); //ordino gli angoli dal più piccolo al piùgrande
        for ( int i = 0; i < n; i++ ){
            coordinate(i, 0) = cos(ang[i]);   //coordinata x della i-esima città
            coordinate(i, 1) = sin(ang[i]);   //coordinata y della i-esima città
            out << i+1 << " " << coordinate(i, 0) << " " << coordinate(i, 1) << endl;
        }
        out.close();
    // Q significa che le città sono distribuite in un quadrato di lato 1, con angolo in basso a sinistra (0,0)
    } else if (schema == 'Q'){
        ofstream out ("./OUTPUT/citta_Q.dat"); //output per le città sul cerchio
        if (!out.is_open()){
            cout << "Error opening file" << endl;
        }
        out << "#" << " " << "X:" << " " << "Y:" << endl; //intestazione del file
        for (int i = 0; i < n; i++){
            coordinate(i, 0) = rnd.Rannyu();   //coordinata x della i-esima città
            coordinate(i, 1) = rnd.Rannyu();   //coordinata y della i-esima città
            out << i+1 << " " << coordinate(i, 0) << " " << coordinate(i, 1) << endl;
        }
        out.close();
    } else{
        cout << "Errore: Le geometrie possibili sono cerchio (C) o quadrato (Q)" << endl;
    }
    //Riempio la matrice delle distanze tra le città (norma L^2)
    //Calcolo tutte le distanze anche se basterebbe calcolare sono gli elementi di metà matrice
    //perchè la matrice è simmetrica
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            distanze(i, j) = pow(coordinate(i, 0) - coordinate(j, 0), 2) + pow(coordinate(i, 1) - coordinate(j, 1), 2);
        }
    }

    return distanze; //restituisco la matrice delle distanze tra le città
}

