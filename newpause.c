/* compile with -l pthread */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <assert.h>

static bool signal_exit = false;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

static void handle_signal(__attribute__((unused)) int sig) {
    signal_exit = true;
}

void * interruptor(__attribute__((unused)) void * arg) {
    sleep(1);

    int success = pthread_mutex_lock(&cond_mutex);
    assert (success == 0);
    signal_exit = true;
    pthread_cond_signal(&cond_var);
    success = pthread_mutex_unlock(&cond_mutex);
    assert (success == 0);

    puts("thread done");
    pthread_exit(NULL);
}

int main(void) {
    signal(SIGINT, handle_signal);

    int success;
    pthread_t thread;
    pthread_create(&thread, NULL, &interruptor, NULL);

    while (!signal_exit) {
        puts("starting loop");
        success = pthread_mutex_lock(&cond_mutex);
        assert (success == 0);
        success = pthread_cond_wait(&cond_var, &cond_mutex);
        assert (success == 0);
        success = pthread_mutex_unlock(&cond_mutex);
        assert (success == 0);
    }

    success = pthread_join(thread, NULL);
    assert (success == 0);

    return 0;
}
