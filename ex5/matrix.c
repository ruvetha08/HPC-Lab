#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **A, **B, **sum, **prod;
int n;

typedef struct {
    int row;
    int col;
} ThreadData;

void *mat_add(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int i = data->row,j;
    for (j = 0; j < n; j++) {
        sum[i][j] = A[i][j] + B[i][j];
    }
    free(data);
    return NULL;
}

void *mat_mul(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int i = data->row,j,k;
    for (j = 0; j < n; j++) {
        prod[i][j] = 0;
        for (k = 0; k < n; k++) {
            prod[i][j] += A[i][k] * B[k][j];
        }
    }
    free(data);
    return NULL;
}

void print_matrix(int **matrix) {
   int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    pthread_t *threads_add, *threads_mul;
    int i, j;
    printf("\n**Pthread program to do matrix addition and multiplication using 2 threads.**\n\n");

    printf("Enter matrix size (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;

    A = malloc(n * sizeof(int *));
    B = malloc(n * sizeof(int *));
    sum = malloc(n * sizeof(int *));
    prod = malloc(n * sizeof(int *));
    threads_add = malloc(n * sizeof(pthread_t));
    threads_mul = malloc(n * sizeof(pthread_t));

    if (!A || !B || !sum || !prod || !threads_add || !threads_mul) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(int));
        B[i] = malloc(n * sizeof(int));
        sum[i] = malloc(n * sizeof(int));
        prod[i] = malloc(n * sizeof(int));

        if (!A[i] || !B[i] || !sum[i] || !prod[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
    }

    printf("Enter elements for Matrix A:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter elements for Matrix B:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    for (i = 0; i < n; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        data->row = i;
        pthread_create(&threads_add[i], NULL, mat_add, data);
    }


    for (i = 0; i < n; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        data->row = i;
        pthread_create(&threads_mul[i], NULL, mat_mul, data);
    }

    for (i = 0; i < n; i++) {
        pthread_join(threads_add[i], NULL);
    }

    for (i = 0; i < n; i++) {
        pthread_join(threads_mul[i], NULL);
    }

    printf("\nMatrix addition result:\n");
    print_matrix(sum);

    printf("\nMatrix multiplication result:\n");
    print_matrix(prod);

    for (i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(sum[i]);
        free(prod[i]);
    }
    free(A);
    free(B);
    free(sum);
    free(prod);
    free(threads_add);
    free(threads_mul);

    return 0;
}
