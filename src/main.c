#define _GNU_SOURCE 
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include "skewDetect.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;



void thread_routine1(void *arg)
{
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: Locked mutex1 and mutex2\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

}

void thread_routine2(void *arg)
{
    pthread_mutex_lock(&mutex2);
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: Locked mutex2 and mutex1\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    
}

/* 主函数: 解析参数、构造请求、启动基准测试 */
int main(int argc,char *argv[])
{
    init_hook();
    pthread_t thread1, thread2;
    /* 构造请求 */
    /* 启动基准测试 */
    pthread_create(&thread1, NULL, thread_routine1, NULL);
    pthread_create(&thread2, NULL, thread_routine2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}


