#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <pthread.h>

typedef struct SignalWrapper{
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t signal_cv;
} SignalWrapper;

int watchdog_create_thread(pthread_t* thread, void* (*th_fun)(void*), pthread_t* wd_thread, void* (*watchdog_func) (void*));

void watchdog_send_signal(SignalWrapper* signal);

#endif // WATCHDOG_H
