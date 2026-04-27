#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    int rows, cols, k, total_sum = 0;
    printf("Enter rows and columns: ");
    scanf("%d %d", &rows, &cols);
    int *matrix = (int *)malloc(rows * cols * sizeof(int));
    printf("Enter elements:\n", rows * cols);
    for (int i = 0; i < rows * cols; i++) {
        scanf("%d", &matrix[i]);
    }
    printf("Enter divisor k: ");
    scanf("%d", &k);
    #pragma omp parallel for reduction(+:total_sum)
    for (int i = 0; i < rows * cols; i++) {
        if (matrix[i] % k == 0) {
            total_sum += matrix[i];
        }
    }
    printf("Parallel Sum of elements divisible by %d: %d\n", k, total_sum);
    free(matrix);
    return 0;
}
