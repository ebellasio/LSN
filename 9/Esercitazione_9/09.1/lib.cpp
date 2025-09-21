#include "lib.h"
#include "random.h"
#include <cmath>
#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

// Funzione che calcola la matrice delle distanze tra le città
// Restituisce una matrice di dimensione n x n, dove n è il numero di città
// La matrice contiene le distanze euclidee tra le città 
mat distanza_citta ( int n, char S, Random &rnd){

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
    //////////////////////////////////////////////////////////////////////////////QUI

    mat distanza (n, n); //matrice che contiene le distanze tra due città 
    mat coordinate_cartesiane (n, 2); //matrice che contiene le coordinate cartesiane tra due città 
   
    //double* citta = new double[n];
    //se C significa che le città sono disposte sulla circonferenza centrata in (0,0) di lato 1
    if (S == 'C'){
        for (int i = 0; i < n; i++){
            double angolo = rnd.Rannyu(0., 2 * M_PI);
            coordinate_cartesiane(i, 0) = cos(angolo);   //coordinata x della i-esima città
            coordinate_cartesiane(i, 1) = sin(angolo);   //coordinata y della i-esima città
            //cout << angolo << endl;       //Prova
        }
   //se Q significa che le città sono distribuite in un quadrato di lato 1, con angolo in basso a sinistra (0,0)
    } else if (S == 'Q'){
        for (int i = 0; i < n; i++){
            coordinate_cartesiane(i, 0) = rnd.Rannyu();   //coordinata x della i-esima città
            coordinate_cartesiane(i, 1) = rnd.Rannyu();   //coordinata y della i-esima città
            cout << coordinate_cartesiane(i, 0) << coordinate_cartesiane(i, 1) << endl;       //Prova
        }
    } else{
        cout << "Le geometrie possibili sono cerchio (C) o quadrato (Q)" << endl;
    }
    //Riempio la matrice delle distanze tra le città (L^2)
    //Calcolo tutte le distanze anche se basterebbe calcolare sono gli elementi di metà matrice
    //perchè la matrice è simmetrica
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            distanza(i, j) = sqrt(pow(coordinate_cartesiane(i, 0) - coordinate_cartesiane(j, 0), 2) + pow(coordinate_cartesiane(i, 1) - coordinate_cartesiane(j, 1), 2));
        }
    }
    //scrivo le coordinate cartesiane in un file di output 
    //TI SERVE CHE SIANO IN UN FILE O BASTANO A VIDEO?
    ofstream coutf;
    string pre (1, S);
    string fout = pre + "_coordinate_cartesiane.dat";
    coutf.open(fout); //,ios::app);
    coutf << "x: " << "y: " << endl;    //Intestazione del file      
    for (int i = 0; i < n; i++){
        coutf << coordinate_cartesiane(i, 0) << " " << coordinate_cartesiane(i, 1) << endl;
    }
    coutf.close();
    //scrivo la matrice delle distanze in un file di output
    string fout_dist = pre + "_distanza_citta.dat";
    coutf.open(fout_dist); //,ios::app);
    coutf << "distanza_citta" << endl;    //Intestazione del file
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            coutf << distanza(i, j) << " ";
        }
        coutf << endl;
    }
    coutf.close();
    return distanza; //restituisco la matrice delle distanze tra le città
}

