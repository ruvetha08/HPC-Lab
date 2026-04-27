#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 100
#define THREADS 4
#define WORD_LEN 50

char *dictionary[] = {"apple", "banana", "cherry", "programming", "linux", "thread"};
int dict_size = 6;

char work_buffer[BUFFER_SIZE][WORD_LEN];
int work_count = 0;
int total_input_done = 0;

char result_buffer[BUFFER_SIZE][100];
int result_count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

int check_dictionary(char word[]) {
    for (int i = 0; i < dict_size; i++) {
        if (strcmp(word, dictionary[i]) == 0)
            return 1;
    }
    return 0;
}

void *spell_check(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (work_count == 0 && !total_input_done) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (work_count == 0 && total_input_done) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        char word[WORD_LEN];
        strcpy(word, work_buffer[--work_count]);
        pthread_mutex_unlock(&mutex);

        int correct = check_dictionary(word);

        pthread_mutex_lock(&mutex);
        if (correct)
            sprintf(result_buffer[result_count++], "%s -> Correct", word);
        else
            sprintf(result_buffer[result_count++], "%s -> Incorrect", word);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, spell_check, NULL);

    printf("Multi-threaded Spell Checker\n");
    printf("Enter words to check ('exit'to stop):\n");

    while (1) {
        char input[WORD_LEN];
        printf("> ");
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) break;

        pthread_mutex_lock(&mutex);
        if (work_count < BUFFER_SIZE) {
            strcpy(work_buffer[work_count++], input);
            pthread_cond_signal(&cond);
        } else {
            printf("Buffer full! Please wait.\n");
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    total_input_done = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("\nSpell Check Results:\n");
    printf("_________________________\n");
    for (int i = 0; i < result_count; i++)
        printf("%s\n", result_buffer[i]);


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
