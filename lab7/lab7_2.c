#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <xmmintrin.h>

#define N 2028  // ����������� �������
#define M 10 // ���������� ������ ���� (��������)

void matrixSum(const float* first, const float* second, float* result) {
    __m128 sum; // ��������� ��������� ���������� ��� �������� 4 float (�����)
    __m128* AA; // � BB ��� ����� 4 ��������� floay
    __m128* BB;
    for (int i = 0; i < N; ++i) {
        AA = (__m128*)(first + i * N); // ����� ��������� �� i-��� ������
        BB = (__m128*)(second + i * N);
        for (int j = 0; j < N / 4; ++j) { // ��� ��� ���������� ����� �� 4 ��������, �� N/4
            sum = _mm_add_ps(AA[j], BB[j]); // ����������
            _mm_store_ps((result + i * N + j * 4), sum); // ��������� � res �� ������ ����� sum ����� �� 4
        }
    }
}

void matrixSub(const float* first, const float* second, float* result) {
    __m128 sub;
    __m128* AA;
    __m128* BB;
    for (int i = 0; i < N; ++i) {
        AA = (__m128*)(first + i * N); // ����� ��������� �� i-��� ������
        BB = (__m128*)(second + i * N);
        for (int j = 0; j < N / 4; ++j) {
            sub = _mm_sub_ps(AA[j], BB[j]); // ��������
            _mm_store_ps(result + i * N + j * 4, sub); // ����������
        }
    }
}

void matrixMult(const float* first, const float* second, float* result) {
    __m128 line, column, temp, sum;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j += 4) {
            sum = _mm_setzero_ps(); // �������� �����
            for (int k = 0; k < N; ++k) {
                column = _mm_set1_ps(first[i * N + k]); // ����� ������� �������
                line = _mm_load_ps(second + k * N + j); // ����� �������� ������ �������
                temp = _mm_mul_ps(column, line); // �������� ��
                sum = _mm_add_ps(sum, temp); // ���������� � ����������� � ����
            }
            _mm_store_ps(result + i * N + j, sum); // ��������� � result
        }
    }
}

float A_1(float* A) {
    float max = 0, tmp = 0;
    for (int i = 0; i < N; ++i) {
        tmp = 0;
        for (int j = 0; j < N; ++j) {
            tmp += abs(A[i * N + j]);
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
            tmp += abs(A[j * N + i]);
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

float* invertMatrix(float* A) {
    float* B = (float*)malloc(N*N*sizeof(float)); // A(�) / a_1 * a_inf
    float* I = (float*)malloc(N*N*sizeof(float)); // ��������� �������
    float* BA = (float*)malloc(N*N*sizeof(float)); // B * A
    float* R = (float*)malloc(N*N*sizeof(float)); // I - BA
    float* buf = (float*)malloc(N*N*sizeof(float)); // buffer
    float* res = (float*)malloc(N*N*sizeof(float)); // result

    // ����. ����� �� �������� � �������
    float a_1 = A_1(A);
    float a_inf = A_inf(A);

    // ���������� ��������� �������
    IMatrixFill(I);
    IMatrixFill(buf); // ���� ���������, ����� � ��� ��������� �������� ��������

    // B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i * N + j] = A[j * N + i] / (a_inf * a_1);
        }
    }

    // R
    matrixMult(A, B, BA);
    matrixSub(I, BA, R);


    for (int k = 0; k < M - 1; ++k) {
        matrixMult(I, R, BA);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
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
//

    printf("Time of program with vectorization: %f seconds\n",(double)(end.tv_sec-start.tv_sec) + 0.000000001*(double)(end.tv_nsec-start.tv_nsec));
    free(A);
    free(Inv);

    return 0;
}