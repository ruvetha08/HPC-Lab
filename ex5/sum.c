#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arr[100];
int n, num;
int total = 0;

void *sum(void *rank) {
    int id = *(int *)rank;
    int size = n / num;
    int start = id * size;
    int end = start + size;
    if (id == num - 1) {
        end = n;
    }

    int local = 0,i;
    printf("Thread %d processing elements: ", id);
    for (i = start; i < end; i++) {
        local += arr[i];
        printf("%d ", arr[i]);
    }
    printf("\n");

    total += local;
    pthread_exit(NULL);
}

int main() {
    int i;
    printf("\n**sum of n elements in the array using pthread**\n\n");
    printf("Enter number of elements: ");
    scanf("%d", &n);
    printf("Enter number of threads: ");
    scanf("%d", &num);


    for (i = 0; i < n; i++) {
        printf("Enter element %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    pthread_t threads[100];
    int thread_id[100];

    for (i = 0; i < num; i++) {
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, sum, &thread_id[i]);
    }

    for (i = 0; i < num; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total: %d\n", total);
    return 0;
}
