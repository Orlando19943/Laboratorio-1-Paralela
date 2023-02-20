/* File:     piSeriesNaive.c
 * Purpose:  Estimate pi using OpenMP and the formula
 *              pi = 4*[1 - 1/3 + 1/5 - 1/7 + 1/9 - . . . ]
 *
 * Compile:  gcc -g -Wall -fopenmp -o piSeriesNaive piSeriesNaive.c -lm
 * Run:      piSeriesNaive <thread_count> <n>
 *           thread_count is the number of threads
 *           n is the number of terms of the series to use
 *
 * Output:   The estimate of pi and the value of pi computed by the
 *           arctan function in the math library
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char *prog_name);

int main(int argc, char *argv[])
{
    long long n, k, j;
    int thread_count;
    double sum1 = 0.0;
    double sum2 = 0.0;

    thread_count = strtol(argv[1], NULL, 10);
    n = strtoll(argv[2], NULL, 10);

    double first_time = omp_get_wtime();
#pragma omp parallel for num_threads(thread_count) schedule(guided, 16)\
    reduction(+ : sum1) reduction(- : sum2)  private(k)
    for (k = 0; k < n/2; k++)
    {
        sum1 += (1.0) / (2 * (k*2) + 1);
        sum2 += -(1.0) / (2 * (2*k+1) + 1);
    }
    
    double second_time = omp_get_wtime();
    sum1 += sum2;
    sum1 = 4.0 * sum1;
    FILE *fp;
    fp = fopen("time3.txt", "a");
    fprintf(fp, "secuencial %f \n", second_time - first_time);
    fclose(fp);
    printf("With n = %lld terms and %d threads,\n", n, thread_count);
    printf("   Our estimate of pi = %.14f\n", sum1);
    printf("                   PI = %.14f\n", 4.0 * atan(1.0));
    printf("Time: %f\n", second_time - first_time);

    return 0;
} /* main */