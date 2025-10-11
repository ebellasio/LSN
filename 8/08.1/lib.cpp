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
H_avg_err H_avg ( Parametri p, Random &rnd){
    double sum = 0.0;
    double sum2 = 0.0;

    double x_new, x_old = 0.0;
    double delta = 0.5; 
    deltaTune (delta, rnd, p);

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
            } else{
                H_sum += H(x_old, p); //energia della vecchia posizione
            }            
        }
        sum += H_sum / double(L);
        sum2 += (H_sum / double(L)) * (H_sum / double(L));   
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

