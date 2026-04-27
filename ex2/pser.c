#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double determinant(int n, double A[n][n]) {
    int i, j, k;
    double det = 1.0;
    int swapCount = 0;
    for (i = 0; i < n; i++) {
        if (A[i][i] == 0) {
            int swapped = 0;
            for (j = i + 1; j < n; j++) {
                if (A[j][i] != 0) {
                    for (k = 0; k < n; k++) {
                        double temp = A[i][k];
                        A[i][k] = A[j][k];
                        A[j][k] = temp;
                    }
                    swapCount++;
                    swapped = 1;
                    break;
                }
            }
            if (!swapped) { det = 0; break; }
        }
        for (j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];
            for (k = i; k < n; k++) A[j][k] -= factor * A[i][k];
        }
    }
    if (det != 0) {
        for (i = 0; i < n; i++) det *= A[i][i];
        if (swapCount % 2 != 0) det = -det;
    }
    return det;
}

int main() {
    int m1, n1, m2, n2, i, j, k;
    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &m1, &n1);
    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &m2, &n2);

    double A[m1][n1], B[m2][n2];

    printf("Enter elements of Matrix A:\n");
    for (i = 0; i < m1; i++) for (j = 0; j < n1; j++) scanf("%lf", &A[i][j]);
    printf("Enter elements of Matrix B:\n");
    for (i = 0; i < m2; i++) for (j = 0; j < n2; j++) scanf("%lf", &B[i][j]);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    if (m1 == m2 && n1 == n2) {
        double add[m1][n1];
        printf("\nMatrix Addition:\n");
        for (i = 0; i < m1; i++) {
            for (j = 0; j < n1; j++) {
                add[i][j] = A[i][j] + B[i][j];
                printf("%.2lf ", add[i][j]);
            }
            printf("\n");
        }
    }

    if (m1 == m2 && n1 == n2) {
        double sub[m1][n1];
        printf("\nMatrix Subtraction:\n");
        for (i = 0; i < m1; i++) {
            for (j = 0; j < n1; j++) {
                sub[i][j] = A[i][j] - B[i][j];
                printf("%.2lf ", sub[i][j]);
            }
            printf("\n");
        }
    }

    if (n1 == m2) {
        double mul[m1][n2];
        printf("\nMatrix Multiplication:\n");
        for (i = 0; i < m1; i++) {
            for (j = 0; j < n2; j++) {
                mul[i][j] = 0;
                for (k = 0; k < n1; k++) mul[i][j] += A[i][k] * B[k][j];
                printf("%.2lf ", mul[i][j]);
            }
            printf("\n");
        }
    } else printf("\nMatrix multiplication not possible\n");

    if (m1 == n1) {
        double copy[m1][n1];
        for (i = 0; i < m1; i++) for (j = 0; j < n1; j++) copy[i][j] = A[i][j];
        printf("\nDeterminant of Matrix A = %.2lf\n", determinant(m1, copy));
    } else printf("\nDeterminant not possible for Matrix A\n");

    if (m2 == n2) {
        double copy[m2][n2];
        for (i = 0; i < m2; i++) for (j = 0; j < n2; j++) copy[i][j] = B[i][j];
        printf("\nDeterminant of Matrix B = %.2lf\n", determinant(m2, copy));
    } else printf("\nDeterminant not possible for Matrix B\n");

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("\nSerial execution CPU time: %.3lf milliseconds\n", elapsed);

    return 0;
}
