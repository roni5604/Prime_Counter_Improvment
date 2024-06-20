#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//this class is used to check if a number is prime from a file and print the result 
//and the total number of prime numbers in the file
//the file is numbers.txt
//how to use:
//  ./randomGenerator 10 100 > numbers.txt
//  gcc -o check_primes check_primes.c
//       ./check_primes

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}


int main() {
    FILE *file = fopen("numbers.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int num;
    int prime_count = 0;

    while (fscanf(file, "%d", &num) != EOF) {
        if (isPrime(num)) {
            printf("%d is prime\n", num);
            prime_count++;
        } else {
            printf("%d is not prime\n", num);
        }
    }

    fclose(file);

    printf("Total primes: %d\n", prime_count);
    return 0;
}
