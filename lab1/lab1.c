#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

double myExp(double x, long long N) {
    double value = 1;
    double tmp = 1;                 
    bool isNegative = false;

    if (x < 0)
        isNegative = true;
    x = fabs(x);

    for (long long step = 1; step <= N; ++step) {
        tmp *= x / (double) step;
        value += tmp;
    }

    if (isNegative)
        value = 1 / value;

    return value;
}

int main(int argc, char* argv[]) {

    struct timespec start, end;

    long long n = strtol(argv[1], NULL,10);
    double x = -0.5;

    clock_gettime(CLOCK_MONOTONIC_RAW,&start);

    double res = myExp(x, n);

    clock_gettime(CLOCK_MONOTONIC_RAW,&end);

    fprintf(stdout,"Time taken: %lf sec.\n",
            (double)(end.tv_sec-start.tv_sec) + 0.000000001*(double)(end.tv_nsec-start.tv_nsec));


    return 0;
}
