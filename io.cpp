#include <mpi.h>
#include <stdio.h>
#include "io.h"

HashMap* load_rule_map(char* path) {
    int count;
    // size of the neighboorhood, 5 for von neumann and 9 for moore
    int neighborhood_size;
    int* buf;
    MPI_Status stat;
    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD, path, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read_all(fh, &count, 1, MPI_INTEGER, &stat);
    MPI_File_read_all(fh, &neighborhood_size, 1, MPI_INTEGER, &stat);
    buf = (int*) malloc(sizeof(int) * count * (neighborhood_size+1));
    printf("%x", buf);
    MPI_File_read_all(fh, buf, count*(neighborhood_size+1), MPI_INTEGER, &stat);
    return parse_rule_map(buf, count, neighborhood_size);
}

HashMap* parse_rule_map(int* buf, int count, int neighborhood_size) {
    HashMap* hm = hm_init(count);
    for (int i = 0; i < count; i++) {
        int* neighborhood = (int*) malloc(sizeof(int)*neighborhood_size); 
        for (int j = 0; j < neighborhood_size; j++) {
            neighborhood[j] = buf[i*(neighborhood_size+1)+j];
        }
        int val = buf[i*(neighborhood_size+1)+neighborhood_size];
        hm_insert(hm, count, neighborhood, neighborhood_size, val);
    }
    return hm;
}

int* load_init_state(char* path, int size) {
    int n, row, col, local_size;
    int* buf;
    int procs, rank;
    MPI_Status stat;
    MPI_File fh;
    MPI_Datatype vec;

    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    n = sqrt(procs);
    local_size = size / n;
    buf = (int*) malloc(sizeof(int) * local_size);

    MPI_Type_vector(local_size, local_size * n, 1, MPI_INTEGER, &vec);
    MPI_Type_commit(&vec);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    row = rank / n;
    col = rank % n;
    MPI_File_open(MPI_COMM_WORLD, path, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_seek(
            fh,
            4 * (local_size * local_size * n * row + local_size * col),
            MPI_SEEK_SET
    );
    MPI_File_read_all(fh, buf, local_size*local_size, MPI_INTEGER, &stat); 
    return buf;
}
