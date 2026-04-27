#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int rows, cols, count = 0;
    printf("Enter rows and columns: ");
    scanf("%d %d", &rows, &cols);

    int matrix[rows][cols];

    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    #pragma omp parallel for reduction(+:count)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int current = matrix[i][j];
            int min_row = 1;
            int max_col = 1;
            for (int k = 0; k < cols; k++) {
                if (matrix[i][k] < current) {
                    min_row = 0;
                    break;
                }
            }
            if (min_row) {
                for (int k = 0; k < rows; k++) {
                    if (matrix[k][j] > current) {
                        max_col = 0;
                        break;
                    }
                }

                if (max_col) {
                    count++;
                }
            }
        }
    }

    printf("Total number of saddle points: %d\n", count);

    return 0;
}
