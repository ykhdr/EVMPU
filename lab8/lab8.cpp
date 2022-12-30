#include <iostream>
#include <cmath>
#include <random>
#include <x86intrin.h>

using namespace std;


// Нужен для того, чтобы разогнать процессор до одиноковой частоты 
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

void straight(unsigned int *array, int N)
{
    for (size_t i = 0; i < N - 1; i++)
    {
        array[i] = i + 1;
    }
    array[N - 1] = 0;
}

void reversed(unsigned int *array, int N)
{
    array[0] = N - 1;
    for (size_t i = 1; i < N; i++)
    {
        array[i] = i - 1;
    }
}

void random(unsigned int *array, int N) {
    for (int i = 0; i < N; i++)
    {
        array[i] = i;
    }
    for (int i = N - 1; i > 0; i--)
    {
        int j = rand() % i;
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

unsigned long long run_through_array(unsigned int *array, int N, int K)
{
    int k = 0;
    for(int i = 0; i < N * K; i++)
    {
        k = array[k];
    }
    if (k == 12345) printf("WOW!");

    unsigned long long begin = __rdtsc();
    for(int i = 0; i < N * K; i++)
    {
        k = array[k];
    }
    unsigned long long end = __rdtsc();
    if (k == 12345) printf("WOW!");

    return (end - begin) / (N * K);
}

int main()
{
    int K = 10;
    unsigned long long D = 0;

    for (int N = 256; N <= 2 * 32 * 1024 * 256; N += (N / 5))
    {
        unsigned int *array = new unsigned int[N];
        straight(array, N);
        D = overclock();
        if (D == 12345) std::cout <<"WOW!";
        std::cout << "Mode: STRAIGHT, Array size: " << N << ", Ticks for one reading: " << run_through_array(array, N, K) << std::endl;
    }

    for (int N = 256; N <= 2 * 32 * 1024 * 256; N += (N / 5))
    {
        unsigned int *array = new unsigned int[N];
        reversed(array, N);
        D = overclock();
        if (D == 12345) std::cout <<"WOW!";
        std::cout << "Mode: REVERSED, Array size: " << N << ", Ticks for one reading: " << run_through_array(array, N, K) << std::endl;
    }

    for (int N = 256; N <= 2 * 32 * 1024 * 256; N += (N / 5))
    {
        unsigned int *array = new unsigned int[N];
        random(array, N);
        D = overclock();
        if (D == 12345) std::cout <<"WOW!";
        std::cout << "Mode: RANDOM, Array size: " << N << ", Ticks for one reading: " << run_through_array(array, N, K) << std::endl;
    }
}
