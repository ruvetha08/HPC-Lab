#include <stdio.h>
#include <omp.h>

int main() {
    int m, n, i, j;
    printf("Enter rows and columns: ");
    scanf("%d %d", &m, &n);

    int a[m][n], res[m][n];
    printf("Enter elements of the matrix:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    #pragma omp parallel for private(i, j)
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            res[i][n - 1 - j] = a[i][j];
        }
    }

    printf("\nMirrored Matrix:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }

    return 0;
}
