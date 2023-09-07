#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 4 * atan(1)

double** DCT_type2_2d (int** matrix, double** DCT, int N1, int N2) {

    double ck1, ck2, sum;

    for (int k1 = 0; k1 < N1; k1++) {
        for (int k2 = 0; k2 < N2; k2++) {
            if (k1 == 0)
                ck1 = 1/sqrt(N1);
            
            else
                ck1 = sqrt(2)/sqrt(N1);
            
            if (k2 == 0)
                ck2 = 1/sqrt(N2);
            
            else
                ck2 = sqrt(2)/sqrt(N2);
            
            sum = 0;
            for (int n1 = 0; n1 < N1; n1++) {
                for (int n2 = 0; n2 < N2; n2++) {
                    sum += matrix[n1][n2] * cos((2*n1+1) * k1*PI/(2*N1)) * cos((2*n2+1) * k2*PI/(2*N2));
                }
            }
            DCT[k1][k2] = ck1 * ck2 * sum;
        }
    }
    return DCT;
}

void display_DCT_coeffs (double** DCT, int N1, int N2) {
    for (int k1 = 0; k1 < N1; k1++) {
        for (int k2 = 0; k2 < N2; k2++)
            printf("%lf\t", DCT[k1][k2]);
        printf("\n");
    }
}


int main () {
    int N1 = 8;
    int N2 = 8;
    
    /* Allocate matrices */
    double** double_matrix = (double**)malloc(N1 * sizeof(double*));
    for (int idx = 0; idx < N1; idx++) {
        double_matrix[idx] = (double*)malloc(N2 * sizeof(double));
    }

    int** int_matrix = (int**)malloc(N1 * sizeof(int*));
    for (int idx = 0; idx < N1; idx++) {
        int_matrix[idx] = (int*)malloc(N2 * sizeof(int));
    }
    for (int k1 = 0; k1 < N1; k1++) {
        for (int k2 = 0; k2 < N2; k2++) {
            int_matrix[k1][k2] = 255;
        }
    }

    /* Call the DCT functions */
    display_DCT_coeffs(DCT_type2_2d(int_matrix, double_matrix, N1, N2), N1, N2);
    
    /* Free the allocated matrices */
    for (int idx = 0; idx < N1; idx++) {
        free(int_matrix[idx]);
        free(double_matrix[idx]);
    }
    free(int_matrix);
    free(double_matrix);
    return 0;
}

