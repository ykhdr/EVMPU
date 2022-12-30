#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

 #define N 2028// размерность матрицы
 #define M 10// количество членов ряда (итераций)

void matrixSum(float *first, float *second, float *result)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            result[i * N + j] = first[i * N + j] + second[i * N + j];
        }
    }
}

void matrixSub(float *first, float *second, float *result)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            result[i * N + j] = first[i * N + j] - second[i * N + j];
        }
    }
}

void matrixMult(float *first, float *second, float *result)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            result[i * N + j] = 0;
            for (int k = 0; k < N; ++k)
            {
                result[i * N + j] += first[i * N + k] * second[k * N + j];
            }
        }
    }
}


float A_1(float *A)
{ // считаем сумма модулей всех a_i по строке и выбираем max
    float max = 0, tmp = 0;
    for (int i = 0; i < N; ++i)
    {
        tmp = 0;
        for (int j = 0; j < N; ++j)
        {
            tmp += abs(A[i * N + j]);
        }
        if (tmp > max)
            max = tmp;
    }
    return max;
}

float A_inf(float *A)
{ // считаем сумму модулей всех a_i о столбцам и выбираем max
    float max = 0, tmp = 0;
    for (int i = 0; i < N; ++i)
    {
        tmp = 0;
        for (int j = 0; j < N; ++j)
        {
            tmp += abs(A[j * N + i]);
        }
        if (tmp > max)
            max = tmp;
    }
    return max;
}

void AMatrixFill(float *A)
{
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            A[i * N + j] = (float)( rand() / (float)(RAND_MAX));
        }
    }
}

void IMatrixFill(float *I)
{ // заполняем единичную матрицу
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            I[i * N + j] = (i == j);
        }
    }
}

float *invertMatrix(float *A)
{
    float *B = (float*)malloc(N*N*sizeof(float)); // A(т) / a_1 * a_inf, где a_1 - норма матрицы 1, a_inf -  норма матрицы беск
    float *I = (float*)malloc(N*N*sizeof(float)); // единичная матрица
    float *BA = (float*)malloc(N*N*sizeof(float)); // B * A
    float *R = (float*)malloc(N*N*sizeof(float)); // I - BA
    float *buf = (float*)malloc(N*N*sizeof(float)); // buffer
    float *res = (float*)malloc(N*N*sizeof(float)); // result

    float a_1 = A_1(A);
    float a_inf = A_inf(A);

    IMatrixFill(I);
    IMatrixFill(buf);

    // B
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            B[i * N + j] = A[j * N + i] / (a_inf * a_1);
        }
    }

    // R
    matrixMult(A, B, BA);
    matrixSub(I, BA, R);

    for (int k = 0; k < M - 1; ++k)
    {
        matrixMult(I, R, BA);
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                I[i * N + j] = BA[i * N + j];
            }
        }
        matrixSum(buf, I, buf);
    }
    matrixMult(buf, B, res);

    free(B);
    free(BA);
    free(I);
    free(R);
    free(buf);
    return res;
}


int main()
{
    srand(time(NULL));

    float *A = (float*)malloc(N*N*sizeof(float));
    float *Inv;

    struct timespec start, end;

    AMatrixFill(A);

    clock_gettime(CLOCK_MONOTONIC_RAW,&start);

    Inv = invertMatrix(A);

    clock_gettime(CLOCK_MONOTONIC_RAW,&end);

//    for (size_t i = 0; i < N; ++i) {
//        for (size_t j = 0; j < N; ++j) {
//            std::cout << Inv[i * N + j] << ' ';
//        }
//        std::cout << std::endl;
//    }

    float *res = (float*)malloc(N*N*sizeof(float)); // result
    matrixMult(A,Inv,res);

    printf("норма 1: %f, норма inf: %f",A_1(res),A_inf(res));

    
    printf("Time of program without manual vectorization: %f seconds \n",(double)(end.tv_sec-start.tv_sec) + 0.000000001*(double)(end.tv_nsec-start.tv_nsec)); 

    free(A);
    free(Inv);

    return 0;
}