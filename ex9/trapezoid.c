#include <stdio.h>
#include <omp.h>
double f(double x) {
    return x * x;
}

int main() {
    double a, b;
    int n;
    double h, total_area = 0.0;
    double middle_sum = 0.0;
    printf("Enter the lower limit (a): ");
    scanf("%lf", &a);
    printf("Enter the upper limit (b): ");
    scanf("%lf", &b);
    printf("Enter the number of trapezoids (n): ");
    scanf("%d", &n);

    h = (b - a) / n;
    #pragma omp parallel for reduction(+:middle_sum)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        middle_sum += f(x);
    }
    total_area = (f(a) + f(b)) / 2.0;
    #pragma omp critical
    {
        total_area += middle_sum;
    }

    total_area = total_area * h;

    printf("\nArea trapezoid: %.2f\n", total_area);

    return 0;
}
