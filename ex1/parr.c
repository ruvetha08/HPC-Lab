#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#define MAT_KEY 3073
#define TIME_KEY 5643

int main() {
    int m, n, p;
    printf("Enter matrix order (m n p): ");
    scanf("%d %d %d", &m, &n, &p);


    int **A = (int **)malloc(m * sizeof(int *));
    int **B = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < m; i++) A[i] = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) B[i] = (int *)malloc(p * sizeof(int));


    srand(time(NULL));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            B[i][j] = rand() % 10;


    int shm_mat = shmget(MAT_KEY, m * p * sizeof(int), IPC_CREAT | 0666);
    int *C = (int *)shmat(shm_mat, NULL, 0);


    int shm_time = shmget(TIME_KEY, m * sizeof(double), IPC_CREAT | 0666);
    double *time_shm = (double *)shmat(shm_time, NULL, 0);

    for (int i = 0; i < m; i++) time_shm[i] = 0.0;


    for (int i = 0; i < m; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            struct timeval start, end;
            gettimeofday(&start, NULL);

            for (int j = 0; j < p; j++) {
                C[i * p + j] = 0;
                for (int k = 0; k < n; k++)
                    C[i * p + j] += A[i][k] * B[k][j];
            }

            gettimeofday(&end, NULL);


            time_shm[i] = (double)(end.tv_sec - start.tv_sec) * 1000.0 +
                          (double)(end.tv_usec - start.tv_usec) / 1000.0;

            exit(0);
        }
    }


    for (int i = 0; i < m; i++) wait(NULL);

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

    if(m<=10 && p<=10)
    {
       printf("\nResult\n");
       int z=0;
       for(int i=0;i<m;i++){
          for(int j=0;j<p;j++){
             printf("%d ",C[z++]);
          }
          printf("\n");
       }
    }


    double parallel_time = 0.0;
    for (int i = 0; i < m; i++)
        if (time_shm[i] > parallel_time)
            parallel_time = time_shm[i];

    printf("computational Time: %.3f ms\n", parallel_time);


    shmdt(C);
    shmdt(time_shm);
    shmctl(shm_mat, IPC_RMID, NULL);
    shmctl(shm_time, IPC_RMID, NULL);

    for (int i = 0; i < m; i++) free(A[i]);
    free(A);
    for (int i = 0; i < n; i++) free(B[i]);
    free(B);

    return 0;
}
