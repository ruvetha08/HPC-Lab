#include <stdio.h>
#include <omp.h>

int main() {
    int n, constant, i;
    printf("Enter the size of the array: ");
    scanf("%d", &n);
    printf("Enter the constant value to add: ");
    scanf("%d", &constant);

    int a[n];

    printf("Enter %d elements: ", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    #pragma omp parallel for private(i)
    for (i = 0; i < n; i++) {
        a[i] = a[i] + constant;
    }

    printf("\nModified Array (after adding %d):\n", constant);
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
