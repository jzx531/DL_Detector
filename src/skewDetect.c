#include "skewDetect.h"
#include "Diagraph.h"

pthread_mutex_lock_t pthread_mutex_lock_f;

pthread_mutex_unlock_t pthread_mutex_unlock_f;

static void lock_before();
static void lock_after();
static void unlock_before();
static void unlock_after();

int pthread_mutex_lock(pthread_mutex_t *mtx)
{
    lock_before();
    pthread_mutex_lock_f(mtx);
    lock_after();

    pthread_t selfid = pthread_self();
    printf("pthread_mutex_lock: %p by thread %p\n", mtx, selfid);
}

int pthread_mutex_unlock(pthread_mutex_t *mtx)
{

    unlock_before();
    pthread_mutex_unlock_f(mtx);    
    unlock_after();
    
    pthread_t selfid = pthread_self();
    printf("pthread_mutex_unlock: %p by thread %p\n", mtx, selfid);
}

int init_hook(void)
{
    if(!pthread_mutex_lock_f){
        pthread_mutex_lock_f = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    }
    if(!pthread_mutex_unlock_f){
        pthread_mutex_unlock_f = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    }
}