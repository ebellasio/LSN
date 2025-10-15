#include "lib.h"
#include "popolazione.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace arma;


//CLASSE CROMOSOMA
//Costruttore -> il primo cromosoma ha le città in ordine crescente
Cromosoma :: Cromosoma (int n){
    this -> _n = n; //numero di geni
    this -> _cromosoma.resize(n); //dimensiona il vettore cromosoma
    for (int i = 0; i < _n; i++){
        _cromosoma[i] = i+1; //cromosoma 0
    }
    if (this->check_passed() == false){
        cout << "Errore: non sono state rispettare le condizioni sull'ordine delle città" << endl;
        return;
    }
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
void Cromosoma::fitness() {
    this->_fitness = 0;
    for (int i = 0; i < _n - 1; i++) {
        int da = _cromosoma[i] - 1; // Se i geni vanno da 1 a n, usa -1 per accedere alla matrice
        int a  = _cromosoma[i+1] - 1;
        this->_fitness += this->_D(da, a);
    }
    // chiudi il ciclo
    int last = _cromosoma[_n - 1] - 1;
    int first = _cromosoma[0] - 1;
    this->_fitness += this->_D(last, first);
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
        int temp = this->_cromosoma[l+i];
        this->_cromosoma[l+i] = this->_cromosoma[l+j+i];
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
    int m = rnd.Rannyu(1, max_m); // lunghezza del blocco da spostare
    int i = rnd.Rannyu(1, _n - 2 * m + 1); // posizione del primo blocco, i in [1, _n-2m]
    //int j = rnd.Rannyu(); // posizione del secondo blocco, j in [i+m, _n-m]
    for (int k = 0; k < m; ++k) {
        int temp = this->_cromosoma[i+k];
        this->_cromosoma[i+k] = this->_cromosoma[i+m+k];
        this->_cromosoma[i+m+k] = temp;
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

//stampo la generazione a cui siamo arrivati e la fitness di ogni individuo della popolazione
void Popolazione :: print_popolazione (ofstream &out){
    for (int i = 0; i<_m; i++){
        out << this->_index_gen << " " << i+1 << " ";
        this->_popolazione[i].print_fitness(out);
        //stamp il cromosoma o la fitness
    }
}

//ordina la popolazione in ordine crescente di fitness
void Popolazione :: Sort(){
    sort(this->_popolazione.begin(), this->_popolazione.end());
}

//Seleziona un individuo dalla popolazione in modo che la probabilità
//di essere scelto sia più alta per gli individui con fitness più bassa
int Popolazione :: Select(Random &rnd) const{
    double r = rnd.Rannyu(); //numero casuale tra 0 e 1
    double p = 2.1;
    int j = (int)(this->_m * pow(r, p)); //indice dell'individuo selezionato
    return j;
}

//Metodo che crea la nuova generazione con anche le mutazioni
Popolazione Popolazione :: NewGenerationCrossover( Popolazione pop_old, Random &rnd){
    Popolazione pop_new = pop_old; //creo la nuova generazione come copia della vecchia
    pop_new.NextGen(); //incremento l'indice della generazione

    Cromosoma figlio1(this->_n); //creo il primo figlio
    Cromosoma figlio2(this->_n); //creo il secondo figlio

    for ( int k = 0; k+1 < this->_m; k+=2){ //faccio crossover m volte per tenere cost il numero di individui
        int i1 = Select(rnd); //seleziono il primo individuo
        int i2;
        do{
            i2 = Select(rnd); //seleziono il secondo individuo
        }while(i1 == i2); //assicuro che i due individui siano diversi
        
        figlio1 = pop_old._popolazione[i1]; //prendo il primo individuo
        figlio2 = pop_old._popolazione[i2]; //prendo il secondo individuo
    
        if (rnd.Rannyu() < this->_p_c){ //faccio il crossover con probabilità p_c
            int punto = rnd.Rannyu(1, this->_n-1); //punto di crossover

            vector<int> temp1 (this->_n, -1); //vettore temporaneo per il primo figlio
            vector<int> temp2 (this->_n, -1); //vettore temporaneo per il secondo figlio

            for ( int i = 0; i < punto; i++ ){
                temp1[i] = figlio1.GetGene(i); //copio fino al punto di crossover
                temp2[i] = figlio2.GetGene(i); //copio fino al punto di crossover
            }
            //figlio 1
            int j = punto;
            /*for ( int i = 0; i < this->_n; i++ ){ //completo il cromosoma del figlio1 con i geni mancanti nell'ordine in cui compaiono nel secondo genitore
                int gene = figlio2.GetGene(i);
                if ( find(temp1.begin(), temp1.end(), gene) == temp1.end() && j < this->_n ){ //se il gene non è già presente
                    temp1[j] = gene;
                    j++;
                }
            }*/
            for ( int i = 0; i < this->_n; i++ ){ 
                bool found = false;
                for (int d = 0; d < punto; d++){ //controllo se il gene è già presente
                    if ( figlio2.GetGene(i) == temp1[d] ){
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    temp1[j] = figlio2.GetGene(i);
                    j++;
                };
            }
            //figlio 2
            j = punto;
            /*for ( int i = 0; i < this->_n; i++ ){ //completo il cromosoma del figlio2 con i geni mancanti nell'ordine in cui compaiono nel secondo genitore
                int gene = figlio1.GetGene(i);
                if ( find(temp2.begin(), temp2.end(), gene) == temp2.end() && j < this->_n ){ //se il gene non è già presente
                    temp2[j2] = gene;
                    j2++;
                }
            }*/
            for ( int i = 0; i < this->_n; i++ ){ 
                bool found = false;
                for (int d = 0; d < punto; d++){ //controllo se il gene è già presente
                    if ( figlio1.GetGene(i) == temp2[d] ){
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    temp2[j] = figlio1.GetGene(i);
                    j++;
                };
            }

            for ( int i = 0; i < this->_n; i++ ){
                if ( temp1[i] == -1 || temp2[i] == -1 ){
                    cout << "Errore nel crossover: gene mancante" << endl;
                    break;
                } else{
                    figlio1.SetGene(i, temp1[i]);
                    figlio2.SetGene(i, temp2[i]);
                }
            }

            this->_popolazione[k] = figlio1; //aggiungo il primo figlio alla nuova popolazione
            this->_popolazione[k+1] = figlio2; //aggiungo il secondo figlio alla nuova popolazione

            if (figlio1.check_passed() == false || figlio2.check_passed() == false){
                cout << "Errore dopo il crossover" << endl;
                break;
            }
        } else {
            //se non faccio il crossover i figli sono copie dei genitori
            pop_new._popolazione[k] = figlio1;
            pop_new._popolazione[k+1] = figlio2;
        } 
    
        if (rnd.Rannyu() < this->_p_m){ //faccio la mutazione con probabilità p_m per il primo individuo
            double r = rnd.Rannyu(); //numero casuale tra 0 e 1
            if ( r < 0.25 ){
                pop_new._popolazione[k].Permutazione(rnd); //mutazione permutazione
            } else if ( r < 0.5 ){
                pop_new._popolazione[k].Shift(rnd); //mutazione shift
            } else if ( r < 0.75 ){
                pop_new._popolazione[k].Scambio(rnd); //mutazione scambio blocchi
            } else {
                pop_new._popolazione[k].Inversione(rnd); //mutazione inversione
            }
        }

        if (rnd.Rannyu() < this->_p_m){ //faccio la mutazione con probabilità p_m per il secondo individuo
            double r = rnd.Rannyu(); //numero casuale tra 0 e 1
            if ( r < 0.25 ){
                pop_new._popolazione[k+1].Permutazione(rnd); //mutazione permutazione
            } else if ( r < 0.5 ){
                pop_new._popolazione[k+1].Shift(rnd); //mutazione shift
            } else if ( r < 0.75 ){
                pop_new._popolazione[k+1].Scambio(rnd); //mutazione scambio blocchi
            } else {
                pop_new._popolazione[k+1].Inversione(rnd); //mutazione inversione
            }
        }
        
        pop_new._popolazione[k].fitness(); //calcolo la fitness di ciascun individuo
        pop_new._popolazione[k+1].fitness(); //calcolo la fitness di ciascun individuo

    }

    pop_new.Sort(); //ordino la popolazione in ordine crescente di fitness per facilitare la selezione nella generazione successiva
    return pop_new; //restituisco la nuova generazione
}