#ifndef SKEWDETECT_H
#define SKEWDETECT_H

// GNU glibc 的扩展宏
#define _GNU_SOURCE 
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

typedef int (*pthread_mutex_unlock_t)(pthread_mutex_t *mtx);

typedef int (*pthread_mutex_lock_t)(pthread_mutex_t *mtx);


int init_hook(void);



#endif // SKEWDETECT_H

