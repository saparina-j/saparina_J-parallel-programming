#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 4) { fprintf(stderr, "Usage: %s N seed out.txt\n", argv[0]); return 1; }
    int n = atoi(argv[1]);
    int seed = atoi(argv[2]);
    const char* out = argv[3];
    srand(seed);
    FILE* f = fopen(out, "w");
    if (!f) { perror("fopen"); return 1; }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(f, "%.12e ", (double)rand() / RAND_MAX);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}