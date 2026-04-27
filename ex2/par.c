#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
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
    int m1, n1, m2, n2;
    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &m1, &n1);
    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &m2, &n2);

    double A[m1][n1], B[m2][n2];
    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < m1; i++) for (int j = 0; j < n1; j++) scanf("%lf", &A[i][j]);
    printf("Enter elements of Matrix B:\n");
    for (int i = 0; i < m2; i++) for (int j = 0; j < n2; j++) scanf("%lf", &B[i][j]);

    int shmid = shmget(1073, 4 * sizeof(double), IPC_CREAT | 0666);
    double *child_times = (double *)shmat(shmid, NULL, 0);
    pid_t p1, p2, p3, p4;

    p1 = fork();
    if (p1 == 0) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        if (m1 == m2 && n1 == n2) {
            printf("\n[Process 1] Addition:\n");
            for (int i = 0; i < m1; i++) {
                for (int j = 0; j < n1; j++) printf("%.2lf ", A[i][j] + B[i][j]);
                printf("\n");
            }
        } else printf("\n[Process 1] Addition not possible\n");
        gettimeofday(&end, NULL);
        child_times[0] = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        exit(0);
    }
    wait(NULL);

    p2 = fork();
    if (p2 == 0) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        if (m1 == m2 && n1 == n2) {
            printf("\n[Process 2] Subtraction:\n");
            for (int i = 0; i < m1; i++) {
                for (int j = 0; j < n1; j++) printf("%.2lf ", A[i][j] - B[i][j]);
                printf("\n");
            }
        } else printf("\n[Process 2] Subtraction not possible\n");
        gettimeofday(&end, NULL);
        child_times[1] = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        exit(0);
    }
    wait(NULL);

    p3 = fork();
    if (p3 == 0) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        if (n1 == m2) {
            printf("\n[Process 3] Multiplication:\n");
            for (int i = 0; i < m1; i++) {
                for (int j = 0; j < n2; j++) {
                    double sum = 0;
                    for (int k = 0; k < n1; k++) sum += A[i][k] * B[k][j];
                    printf("%.2lf ", sum);
                }
                printf("\n");
            }
        } else printf("\n[Process 3] Multiplication not possible\n");
        gettimeofday(&end, NULL);
        child_times[2] = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        exit(0);
    }
    wait(NULL);

    p4 = fork();
    if (p4 == 0) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        if (m1 == n1) {
            double copyA[m1][n1];
            for (int i = 0; i < m1; i++) for (int j = 0; j < n1; j++) copyA[i][j] = A[i][j];
            printf("\n[Process 4] Determinant of A = %.2lf\n", determinant(m1, copyA));
        }
        if (m2 == n2) {
            double copyB[m2][n2];
            for (int i = 0; i < m2; i++) for (int j = 0; j < n2; j++) copyB[i][j] = B[i][j];
            printf("[Process 4] Determinant of B = %.2lf\n", determinant(m2, copyB));
        }
        gettimeofday(&end, NULL);
        child_times[3] = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        exit(0);
    }
    wait(NULL);

    double max_time = child_times[0];
    for (int i = 1; i < 4; i++) if (child_times[i] > max_time) max_time = child_times[i];

    printf("\nChild execution times (ms):\n");
    for (int i = 0; i < 4; i++) printf("Process %d: %.3lf ms\n", i + 1, child_times[i]);
    printf("\nParallel execution time: %.3lf ms\n", max_time);

    shmdt(child_times);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
