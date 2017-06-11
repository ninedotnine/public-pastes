#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

static bool signal_exit = false;

static void handle_signal(__attribute__((unused)) int sig) {
    signal_exit = true;
}

void * interruptor(__attribute__((unused)) void * arg) {
    pthread_detach(pthread_self());
    sleep(1);
    raise(SIGINT);
    pthread_exit(NULL);
}

int main(void) {
    signal(SIGINT, handle_signal);

    pthread_t thread;
    pthread_create(&thread, NULL, &interruptor, NULL);

    while (!signal_exit) {
        pause();
    }

    return 0;
}
