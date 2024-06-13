#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <seed> <count>\n", argv[0]);
        return 1;
    }

    int seed = atoi(argv[1]);
    int count = atoi(argv[2]);

    srand(seed);
    
    int lowerLimit = 1000000;
    int upperLimit = 2100000000;

    for (int i = 0; i < count; ++i) {
        int random_number = rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
        printf("%d\n", random_number);
    }

    return 0;
}
