#include <stdlib.h>
#include <stdio.h>

#include "../include/watchdog.h"

int watchdog_create_thread(pthread_t* thread, void* (*th_fun)(void*), pthread_t* wd_thread, void* (*watchdog_func) (void*)){
    SignalWrapper* signal = malloc(sizeof(*signal));
    if(signal == NULL) {
        return -1;
    }

    *signal = (SignalWrapper){
      .mutex = PTHREAD_MUTEX_INITIALIZER,
      .signal_cv = PTHREAD_COND_INITIALIZER,
      .thread = *thread
    };

    if(pthread_create(thread, NULL, th_fun, signal) != 0) {
        pthread_mutex_destroy(&signal->mutex);
        pthread_cond_destroy(&signal->signal_cv);
        free(signal);
        return -1;
    }
 
    if(pthread_create(wd_thread, NULL, watchdog_func, signal) != 0) {
        pthread_mutex_destroy(&signal->mutex);
        pthread_cond_destroy(&signal->signal_cv);
        free(signal);
        return -1;
    }

    return 0;    
}

void watchdog_send_signal(SignalWrapper* signal) {
    pthread_mutex_lock(&signal->mutex);
    pthread_cond_signal(&signal->signal_cv);
    pthread_mutex_unlock(&signal->mutex);
}
