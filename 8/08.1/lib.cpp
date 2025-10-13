#include "lib.h"

using namespace std;

const int N = 100; //numero di blocchi
const int M = 100000; //numero di step totale
const int L = M/N; //numero di step per blocco

//funzione per calcolare l'incertezza statistica
double error( double av, double av2, int n ){
    if ( n == 0 ) return 0.0;
    else return sqrt( (av2 - av*av ) / double(n) );
};

//funzione d'onda gaussiana
double psi_T ( double x, Parametri p ){
    return exp( -pow(x-p.mu, 2) / (2*pow(p.sigma, 2)) ) + exp( -pow(x+p.mu, 2) / (2*pow(p.sigma, 2)) );
};

//energia stimata dello stato fondamentale
double H (double x, Parametri p){
    double T = - ((pow(x - p.mu, 2) - pow(p.sigma, 2)) * exp(-pow(x-p.mu, 2)/(2*p.sigma*p.sigma)) + (pow(x + p.mu, 2) - pow(p.sigma, 2))
                * exp(-pow(x+p.mu, 2)/(2*p.sigma*p.sigma)))/(2 * pow(p.sigma, 4) * psi_T(x, p)); //energia cinetica
    double V = pow(x, 4) - 2.5*pow(x, 2); // energia potenziale
    return T + V; // energia totale 
}

//metodo di Metropolis per campionare la x nella funzione d'onda
bool Metropolis_x ( double x_new, double x_old, Parametri p, Random &rnd ){
    bool acc = false;
    //Metropolis ritorna il minimo tra 1 e il rapporto tra le due probabilita'
    //(la probabilita' e' proporzionale al quadrato della funzione d'onda)
    double acceptance = min (1.0, pow(psi_T(x_new, p), 2) / pow(psi_T(x_old, p), 2) );
    if ( rnd.Rannyu() < acceptance ) acc = true;
    return acc;
}

//metodo di Metropolis per campionare i parametri della funzione d'onda -> Simulated Annealing
bool Metropolis_param ( double H_new, double H_old, double beta, Random &rnd ){
    bool acc = false;
    //Metropolis ritorna il minimo tra 1 e il rapporto tra le due probabilita'
    double acceptance = exp(-beta * (H_new - H_old));
    if ( rnd.Rannyu() < acceptance ) acc = true;
    return acc;
}

//funzione che calcola la media e l'errore dell'energia
//aggiungo il parametro print per decidere se stampare o no i valori di Energia e x campionati
//mi serve per stampare il valore di energia associato ai parametri ottimali alla fine del simulated annealing
H_avg_err H_avg ( Parametri p, Random &rnd, bool print){
    double sum = 0.0;
    double sum2 = 0.0;

    double x_new, x_old = 0.0;
    double delta = 0.5; 
    deltaTune (delta, rnd, p);

    static ofstream outGS("output_energy.dat"); //static serve per non reinizializzare il file di output ad ogni chiamata della funzione
    //static ofstream outx("sampled_x.dat");

    //inizializzo il file di output
    if(print){
        outGS << "#BLOCK:" << " " << "ENERGY:" << " " << "ERROR:" << endl;
    }

    for (int i=0; i<N; i++){
        double H_sum = 0.0;
        
        //fase di equilibrazione iniziale per lasciare che il sistema si equilibri
        for (int k=0; k<1000; k++){
            x_new = x_old + delta * ( 2 * rnd.Rannyu() - 1 );
            if ( Metropolis_x(x_new, x_old, p, rnd) ) x_old = x_new;
        }

        //fase di campionamento vero e proprio
        //dentro ogni blocco faccio L step di Metropolis
        //e calcolo la media dell'energia in quel blocco
        for (int j=0; j<L; j++){
            x_new = x_old + delta * ( 2 * rnd.Rannyu() - 1 );
            if ( Metropolis_x(x_new, x_old, p, rnd) ) {
                x_old = x_new; //accetto la nuova posizione
                H_sum += H(x_new, p); //energia della nuova posizione
                //if (print) outx << x_new << endl; //stampo il valore di x campionato
            } else{
                H_sum += H(x_old, p); //energia della vecchia posizione
                //if (print) outx << x_old << endl; //stampo il valore di x campionato
            }            
        }
        sum += H_sum / double(L);
        sum2 += (H_sum / double(L)) * (H_sum / double(L));   

        //stampo il valore di energia e il suo errore ad ogni blocco
        if(print) outGS << i+1 << " " << sum/double(i+1) << " " << error(sum/double(i+1), sum2/double(i+1), i) << endl;
    }

    sum /= double(N); //media su tutti i blocchi
    sum2 /= double(N); //media dei quadrati su tutti i blocchi
    double err = error(sum, sum2, N); //incertezza statistica

    return {sum, err};
}

//funzione che adatta il delta per tenere un tasso di accettazione del 50%
//perchè il metropoli campiona bene tutto lo spazio con questo tasso di accettazione
void deltaTune ( double &delta, Random &rnd, Parametri p){

    const double accettazione_standard = 0.5;
    const double tolleranza = 0.01;
    
    double tasso;
    double x_new, x_old = 0.0;

    for ( int i = 0; i < 1000; i++ ){
        int count = 0.0;
        for ( int j = 0; j < 1000; j++ ){
            x_new = x_old + delta * ( 2.0*rnd.Rannyu() - 1.0 );
            if ( Metropolis_x(x_new, x_old, p, rnd) ) {
                x_old = x_new; //accetto la nuova posizione
                count++; //aumento il contatore
            }
        }
        tasso = (double)count / (double)1000;

        if ( fabs(tasso - accettazione_standard) <= tolleranza ) break; //se l'accettazione è quella desiderata ho trovato la delta
        if ( tasso < (accettazione_standard - tolleranza) ) delta *= 0.9;
        if ( tasso > (accettazione_standard - tolleranza) ) delta *= 1.1;
    }
}

//funzione che implementa uno step di simulated annealing per ottimizzare i parametri della funzione d'onda
//ovvero, il for sta nel main
Parametri simulatedAnnealing ( double temp, Parametri p_old, Random &rnd ){

    double beta = 1.0 / temp; // Inizializza i parametri
    double delta_par = 0.5*temp; // Passo per i parametri
    Parametri p_new = p_old; // Copia i parametri vecchi in quelli nuovi

    p_new.mu = fabs(p_old.mu + delta_par * (2.0 * rnd.Rannyu() - 1.0)); // Aggiorna mu
    do{
        p_new.sigma = fabs(p_old.sigma + delta_par * (2.0 * rnd.Rannyu() - 1.0)); // Aggiorna sigma
    } while (p_new.sigma <= 0.2); // Evita sigma troppo piccola che darebbe un campionamento inefficiente

    H_avg_err H_old = H_avg(p_old, rnd, false); // Calcola l'energia media con i parametri vecchi
    H_avg_err H_new = H_avg(p_new, rnd, false); // Calcola l'energia media con i parametri nuovi

    if ( Metropolis_param(H_new.H_avg, H_old.H_avg, beta, rnd) ) {
        return p_new; // Accetta i nuovi parametri
    } else {
        return p_old; // Mantiene i vecchi parametri
    }
    
}
