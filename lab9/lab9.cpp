#include <iostream>
#include <stdlib.h>
#include <random>
#include <x86intrin.h>
#define CACHE_1 65536// 64 KB
#define CACHE_2 262144// 256 KB
#define CACHE_3 12582912// 12 MB

using namespace std;

unsigned long long overclock() {
    unsigned long long matrix1[250][250];
    unsigned long long matrix2[250][250];
    unsigned long long matrix3[250][250];
    for (unsigned long long i = 0; i < 250; i++)
    {
        for (unsigned long long l = 0; l < 250; l++)
        {
            matrix1[i][l] = rand() % 100;
            matrix2[i][l] = rand() % 100;
            matrix3[i][l] = 0;
        }
    }
    for (unsigned long long i = 0; i < 250; i++)
    {
        for (unsigned long long l = 0; l < 250; l++)
        {
            for (unsigned long long j = 0; j < 250; j++)
            {
                matrix3[i][l] += matrix1[i][l] * matrix2[l][j];
            }
        }
    }
    return matrix3[249][249];
}

int count(int *array, unsigned long long size) {
    unsigned long long min = -1;
    for (int l = 0; l < 10; l++) {
        unsigned long long D = overclock();
        if (D == 12345) std::cout <<"WOW!";
        int k = 0;
        for (unsigned long long i = 0; i < size; i++) {
            k = array[k];
        }
        if (k == 12345) printf("Wow!");
        unsigned long long start = __rdtsc();
        for (unsigned long long i = 0; i < size; i++) {
            k = array[k];
        }
        unsigned long long end = __rdtsc();
        if (k == 12345) printf("Wow!");
        if (min == -1) {
            min = (end - start);
        }
        else {
            if (min >= end - start) {
                min = end - start;
            }
        }
    }
    return (min / size);
}

int main(int argc, char **argv) {
    int size = (CACHE_1 + CACHE_2 + CACHE_3) / sizeof(int);
    int offset = 4194304; // 4 ла
    for (int n = 1; n <= 32; n++) {
        int *array = (int *) malloc(offset * n * sizeof(int));
        for (int i = 0; i < offset * n; i++) {
            array[i] = 0;
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < ( size / n ); j++) {
                array[offset * i + j] = (i + 1) * offset + j;
            }
        }
        for (int j = 0; j < ( size / n ); j++) {
            array[ offset * (n - 1) + j ] = j + 1;
        }
        array[ offset * ( n - 1 ) + ( size / n ) -1 ] = 0;
        std::cout << "for " << n << " fragments: " << count(array, size * n) << " ticks\n";
        free(array);
    }
    return 0;
}
