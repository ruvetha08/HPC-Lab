#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i;
    printf("Enter the size of the arrays: ");
    scanf("%d", &n);
    int a[n], b[n], c[n];
    printf("Enter elements for Array A: ");
    for (i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("Enter elements for Array B: ");
    for (i = 0; i < n; i++) scanf("%d", &b[i]);
    #pragma omp parallel for private(i)
    for (i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    printf("\nResultant Array C (A + B):\n");
    for (i = 0; i < n; i++) {
        printf("%d ", c[i]);
    }
    printf("\n");

    return 0;
}
