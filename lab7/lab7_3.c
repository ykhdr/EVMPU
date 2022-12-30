#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#ifdef __INTEL_COMPILER
#include <mkl_cblas.h>
#else
#include <mkl.h>
#endif

#define N 2048  // размерность матрицы
#define M 10 // количество членов ряда (итераций)

float A_1(float* A) {
    float max = 0, tmp = 0;
    for (int i = 0; i < N; ++i) {
        tmp = 0;
        for (int j = 0; j < N; ++j) {
            tmp += fabsf(A[i * N + j]);
        }
        if (tmp > max) max = tmp;
    }
    return max;
}

float A_inf(float* A) {
    float max = 0, tmp = 0;
    for (int i = 0; i < N; ++i) {
        tmp = 0;
        for (int j = 0; j < N; ++j) {
            tmp += fabsf(A[j * N + i]);
        }
        if (tmp > max) max = tmp;
    }
    return max;
}

void IMatrixFill(float* I) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            I[i * N + j] = (i == j);
        }
    }
}

void AMatrixFill(float *A){
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            A[i * N + j] = (float)( rand() /(float)(RAND_MAX));
        }
    }
}

float* invertMatrix(float* A) {
    float* I = (float*)malloc(N * N *sizeof(float)); // единичная матрица
    float* R = (float*)malloc(N * N *sizeof(float)); // I - BA
    float* tmp1 =  (float*)malloc(N * N *sizeof(float));
    float* tmp2 =  (float*)malloc(N * N *sizeof(float));
    float* B = (float*)malloc(N*N*sizeof(float));
    float* BA = (float*)malloc(N*N*sizeof(float));
    float* res = (float*)malloc(N*N*sizeof(float)); // result 
    float* tmp3 = (float*)malloc(N*N*sizeof(float)); // result 
    // макс. сумма по столбцам и строкам
    float a_1 = A_1(A);
    float a_inf = A_inf(A);

    // заполнение единичной матрицы
    IMatrixFill(I);
    IMatrixFill(R);
    IMatrixFill(res);
    IMatrixFill(tmp1);
    IMatrixFill(tmp2);
    //IMatrixFill(A);  
  
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            tmp3[i*N + j] = 0.0;
        }
    }


     // clbas_sgemm - вычислияет C = \alpha AB + \beta C, s - вещ тип один точности
    // 1) - порядок следования элементов: CblasRowMajor - матрицы хранятся по строкам
    // 2,3) - предварительные операции: CblasNoTrans - ничего не делать,  CblasTrans - транспонировать (A, B ) соответсвтенно
    // 4,5,6) - размеры матриц 
    // 7) - коэф alpha 
    // 8) - матрица A
    // 9) - число элементов в строке матрицы A 
    // 10) - матрица B
    // 11) - число элементов в строке матрицы B
    // 12) - кэф beta 
    // 13) - матрица C
    // 14) - число эл в строке матрицы C

    // считаем матрицу B
    cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, N, N, N, 1.0 / (a_1 * a_inf), A, N, I, N, 0.0, B, N);

    // считаем матрицу BA
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, B, N, A, N, 0.0, BA, N);

    // считаем матррицу R
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, -1.0, BA, N, I, N, 1.0, R, N);



    for (int k = 0; k < M - 1; ++k) {
        // умножаем матрицы, получаем tmp2 = R^n
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, tmp1, N, R, N, 0.0, tmp2, N);
        // tmp1 = I* tmp2
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, I, N, tmp2, N, 0.0, tmp1, N);
        // tmp3 = I*tmp1 + tmp3;
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, I, N, tmp2, N, 1.0, tmp3, N);
    }
    // tmp3 = tmp3 + I
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, I, N, I, N, 1.0, tmp3, N);

    // res = res*B
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, tmp3, N, B, N, 0.0, res, N);

/*
for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
             printf("%f ",tmp3[i * N + j]);
        }
        printf("\n");
    }
*/

    free(tmp1);
    free(tmp2);
    free(I);
    free(R);
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
    
    float *res = (float*)malloc(N*N*sizeof(float)); // result
    
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, Inv, N, A, N, 0.0, res, N);


  /*  
 for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
             printf("%f ",res[i * N + j]);
        }
        printf("\n");
    }
*/

    printf("norma 1: %f, norma inf: %f\n",A_1(res),A_inf(res));

    printf("Time of program with BLASS library : %f seconds\n", (double)(end.tv_sec-start.tv_sec) + 0.000000001*(double)(end.tv_nsec-start.tv_nsec));
    
    free(A);
    free(Inv);

    return 0;
}