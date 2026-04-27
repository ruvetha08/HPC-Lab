#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    int m, n, p;
    printf("Enter matrix order (m n p): ");
    if (scanf("%d %d %d", &m, &n, &p) != 3) return 1;


    int **A = (int **)malloc(m * sizeof(int *));
    int **B = (int **)malloc(n * sizeof(int *));
    int **C = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        A[i] = (int *)malloc(n * sizeof(int));
        C[i] = (int *)malloc(p * sizeof(int));
    }
    for (int i = 0; i < n; i++)
        B[i] = (int *)malloc(p * sizeof(int));


    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            B[i][j] = rand() % 10;


    struct timeval start, end;
    gettimeofday(&start, NULL);


    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }

    gettimeofday(&end, NULL);

    if(m<=10&&n<=10){
       printf("\nMatrix A\n");
       for(int i=0;i<m;i++){
          for(int j=0;j<n;j++){
             printf("%d ",A[i][j]);
          }
          printf("\n");
       }
    }

    if(n<=10&&p<=10){
       printf("\nMatrix B\n");
       for(int i=0;i<n;i++){
          for(int j=0;j<p;j++){
             printf("%d ",B[i][j]);
          }
          printf("\n");
       }
    }


    if(m<=10&&p<=10){
      printf("\nResult\n");
       for(int i=0;i<m;i++){
          for(int j=0;j<p;j++){
             printf("%d ",C[i][j]);
          }
          printf("\n");
       }
    }

    double time_spent_ms = (double)(end.tv_sec - start.tv_sec) * 1000.0 +
                           (double)(end.tv_usec - start.tv_usec) / 1000.0;

    printf("computation time: %.3f ms\n", time_spent_ms);


    for (int i = 0; i < m; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (int i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
