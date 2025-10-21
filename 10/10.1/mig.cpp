void migrazione_circolare(Popolazione &pop, int rank, int size, mat D) {
    Cromosoma best = pop.GetBest();
    vector<int> send_info = best.Serialize();
    vector<int> recv_info(send_info.size());

    int dest = (rank + 1) % size;          // processo successivo (con wrapping)
    int source = (rank - 1 + size) % size; // processo precedente (con wrapping)

    MPI_Sendrecv(send_info.data(), send_info.size(), MPI_INT, dest, 0,
                 recv_info.data(), recv_info.size(), MPI_INT, source, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    Cromosoma received(pop.GetN());
    received.Deserialize(recv_info);
    received.Set_distances(D);
    received.fitness();

    pop.SetBest(received); // Sostituisco il migliore con quello ricevuto
    pop.Sort();
}
