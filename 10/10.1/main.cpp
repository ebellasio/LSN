#include "lib.h"
#include <iostream>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

int main(int argc, char *argv[]) {

    //Inizializzo MPI
    int rank, size;
    MPI_Init(&argc, &argv);                   // Avvia l'ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Ottiene l'ID (rank) del processo
    //cout << "Process " << rank << " started." << endl;
    MPI_Comm_size(MPI_COMM_WORLD, &size);     // Ottiene il numero totale di processi
    //cout << "Number of processes: " << size << endl;
    
    //Random rnd;
    // Inizializzazione del generatore di numeri casuali: leggo i semi e i numeri primi dai rispettivi file
    /*int seed[4];
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
        cerr << "PROBLEM: Unable to open seed.in" << endl;*/

    //Inizializzo il generatore di numeri casuali in modo diverso per ogni processo
    Random rnd;
    int seed[4];
    int p1, p2;
    // Lettura primi numeri
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2;
        Primes.close();
    } else {
        cerr << "PROBLEM: Unable to open Primes" << endl;
    }

    // Processo 0 legge seed da file e imposta Random
    if(rank == 0) {
    ifstream input("seed.in");
    string property;
        if (input.is_open()){
            while (!input.eof()){
                input >> property;
                if (property == "RANDOMSEED"){
                    input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                    }
            }
            input.close();
        } else {
            cerr << "PROBLEM: Unable to open seed.in" << endl;
        }

        // Inizializza rnd con seed letto
        rnd.SetRandom(seed, p1, p2);

        // Prepara seed per processo successivo
        int next_seed[4];
        for(int i=0; i<4; i++) {
            next_seed[i] = (seed[i] + 1) % 100000;  // incremento
        }

        // Invia seed al processo 1 se esiste
        if(size > 1) {
            MPI_Send(next_seed, 4, MPI_INT, 1, 0, MPI_COMM_WORLD);
        } 
    } else {
        // Ricevo seed dal processo precedente
        int recv_seed[4];
        MPI_Recv(recv_seed, 4, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Inizializzo rnd con il seed ricevuto
        rnd.SetRandom(recv_seed, p1, p2);

        // Se non sono l'ultimo processo, preparo seed per il prossimo processo
        if(rank < size - 1) {
        int next_seed[4];
        for(int i=0; i<4; i++) {
            next_seed[i] = (recv_seed[i] + 1) % 100000;
            }
        MPI_Send(next_seed, 4, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        }
    }

    ///////////////////////////////////////////////////////////////

    int geni = 34; //numero di città
    int individui = 200; //numero di individui/cromosomi in una generazione
    double p_m = 0.08; //probabilità di mutazione
    double p_c = 0.7; //probabilità di crossover
    int n_generazioni = 600; //numero di generazioni

    bool b_migrazione = true; //se true attivo la migrazione tra i processi ogni 20 generazioni
    int n_migrazione = 20; //frequenza di migrazione tra i processi

    //CIRCONFERENZA
    char geometry = 'C'; // C significa che le città sono disposte sulla circonferenza centrata in (0,0) di lato 1

    mat dist_C = distanze_citta(geni, geometry, rnd, rank); // distanza tra le città poste sulla circonferenza
    Popolazione Pop_C(geni, individui, p_m, p_c, dist_C, rnd); //creo la popolazione iniziale
    Pop_C.Sort(); //ordino la popolazione in ordine crescente di fitness

    ofstream out_pop_C ("./OUTPUT/popolazione_C_" + to_string(rank) + ".dat"); //output per la popolazione
    if (!out_pop_C.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    out_pop_C << "#" << " " << "Generazione:" << " " << "Individuo:" << " " << "Fitness:" << endl; //intestazione del file popolazione.dat
    Pop_C.print_popolazione(out_pop_C); //stampo la popolazione iniziale

    for ( int i=0; i<n_generazioni; i++ ){
        Pop_C = Pop_C.NewGenerationCrossover(Pop_C, rnd); //creo la nuova generazione
        //if(rank == 0) cout << "C " << Pop_C.GetGen() << endl;
        //Migrazione tra i processi ogni n_migrazione generazioni se c'è più di un processo
        if ( b_migrazione == true && (i+1) % n_migrazione == 0 && size > 1){ 
            migrazione(Pop_C, rank, size, dist_C, rnd);
        }
        Pop_C.print_popolazione(out_pop_C); //stampo la nuova generazione
    }

    ofstream out_best_C ("./OUTPUT/migliore_C_" + to_string(rank) + ".dat"); //output per il migliore individuo
    if (!out_best_C.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    Pop_C.GetBest().print_cromosoma(out_best_C); //stampo il migliore individuo per ciascuna generazione

    out_best_C.close();
    out_pop_C.close();

    //QUADRATO
    geometry = 'Q'; // Q significa che le città sono disposte su un quadrato con lato tra -1 e 1

    mat dist_Q = distanze_citta(geni, geometry, rnd, rank); // distanza tra le città poste sul quadrato
    Popolazione Pop_Q(geni, individui, p_m, p_c, dist_Q, rnd); //creo la popolazione iniziale
    Pop_Q.Sort(); //ordino la popolazione in ordine crescente di fitness

    ofstream out_pop_Q ("./OUTPUT/popolazione_Q_" + to_string(rank) + ".dat"); //output per la popolazione
    if (!out_pop_Q.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    out_pop_Q << "#" << " " << "Generazione:" << " " << "Individuo:" << " " << "Fitness:" << endl; //intestazione del file popolazione.dat
    Pop_Q.print_popolazione(out_pop_Q); //stampo la popolazione iniziale

    for ( int i=0; i<n_generazioni; i++ ){
        Pop_Q = Pop_Q.NewGenerationCrossover(Pop_Q, rnd); //creo la nuova generazione
        //if(rank == 0) cout << "Q " << Pop_Q.GetGen() << endl;
        //Migrazione tra i processi ogni n_migrazione generazioni se c'è più di un processo
        if ( b_migrazione == true && (i+1) % n_migrazione == 0 && size > 1){ 
            migrazione(Pop_Q, rank, size, dist_Q, rnd);
        }
        Pop_Q.print_popolazione(out_pop_Q); //stampo la nuova generazione
    }

    ofstream out_best_Q ("./OUTPUT/migliore_Q_" + to_string(rank) + ".dat"); //output per il migliore individuo
    if (!out_best_Q.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    Pop_Q.GetBest().print_cromosoma(out_best_Q); //stampo il migliore individuo per ciascuna generazione

    out_best_Q.close();
    out_pop_Q.close();

    //CAPOLUOGHI DI PROVINCIA ITALIANI
    
    geometry = 'I'; // I significa che le città sono i capoluoghi di provincia italiani
    geni = 110; //numero di città

    mat dist_I = distanze_citta(geni, geometry, rnd, rank); // distanza tra le città poste sul quadrato
    Popolazione Pop_I(geni, individui, p_m, p_c, dist_Q, rnd); //creo la popolazione iniziale
    Pop_I.Sort(); //ordino la popolazione in ordine crescente di fitness

    ofstream out_pop_I ("./OUTPUT/popolazione_I_" + to_string(rank) + ".dat"); //output per la popolazione
    if (!out_pop_I.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    out_pop_I << "#" << " " << "Generazione:" << " " << "Individuo:" << " " << "Fitness:" << endl; //intestazione del file popolazione.dat
    Pop_I.print_popolazione(out_pop_I); //stampo la popolazione iniziale

    n_generazioni = 1000; //numero di generazioni nel caso delle province italiane

    for ( int i=0; i<n_generazioni; i++ ){
        Pop_I = Pop_I.NewGenerationCrossover(Pop_I, rnd); //creo la nuova generazione
        //if(rank == 0) cout << "I " << Pop_I.GetGen() << endl;
        //Migrazione tra i processi ogni n_migrazione generazioni se c'è più di un processo
        if ( b_migrazione == true && (i+1) % n_migrazione == 0 && size > 1){ 
            for (int j = 0; j < size; j++) {
                cout << "Processo " << j << endl;
                migrazione(Pop_I, rank, size, dist_I, rnd);
            }
        }
        Pop_I.print_popolazione(out_pop_I); //stampo la nuova generazione
    }

    ofstream out_best_I ("./OUTPUT/migliore_I_" + to_string(rank) + ".dat"); //output per il migliore individuo
    if (!out_best_I.is_open()){
        cout << "Error opening file" << endl;
        return -1;
    }

    Pop_I.GetBest().print_cromosoma(out_best_I); //stampo il migliore individuo per ciascuna generazione

    out_best_I.close();
    out_pop_I.close();

    MPI_Finalize(); // Termina l'ambiente MPI

    rnd.SaveSeed();
    return 0;
    
}