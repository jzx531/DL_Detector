#include "skewDetect.h"
#include "Diagraph.h"


pthread_mutex_lock_t pthread_mutex_lock_f;

pthread_mutex_unlock_t pthread_mutex_unlock_f;

extern task_graph *tg;

static lock_before(unsigned long int thread_id, unsigned long int lock) 
{
   pthread_mutex_lock_f(&tg->mutex);
   int idx = search_lock(lock);
//    printf("lock_before idx:%d \n", idx);
   //如果是第一次加锁，则不作处理
   if(idx != -1)
   {
        //u是想要加锁的线程
        pair_t u;
        u.th_id = thread_id;
        u.type = PROCESS;
        //把vertex添加到vertex_list里面
        add_vertex(u);
        //v是锁原来的线程
        pair_t v;
        v.th_id = tg->lock_list[idx].th_id;
        tg->lock_list[idx].degrees++;
        v.type = PROCESS;
        add_vertex(v);

        if(!find_edge(u,v))
        {
            add_edge(u,v);
        }
   }
   pthread_mutex_unlock_f(&tg->mutex);
} 

static inline void inc(int *value, int add)
{
    __sync_fetch_and_add(value, add);
}

static void lock_after(unsigned long int thread_id, unsigned long int lock) 
{
    pthread_mutex_lock_f(&tg->mutex);

    int idx = search_lock(lock);
    // printf("lock_after idx:%d \n", idx);
    if (idx == -1) {  // 第一次加锁,找一个空位lock_list，设置th_id和lock
        int empty_lock_idx = search_empty_lock(lock);
        tg->lock_list[empty_lock_idx].th_id = thread_id;
        tg->lock_list[empty_lock_idx].lock_id = lock;
        // printf("tg->lock_num:%d \n", tg->lock_num);
        if (empty_lock_idx >= tg->lock_num) {
            inc(&tg->lock_num, 1);
            // tg->lock_num++;
        }
    }
    else {
        //u是想要加锁的线程
        pair_t u;
        u.th_id = thread_id;
        u.type = PROCESS;
        //v是锁原来的线程
        pair_t v;
        v.th_id = tg->lock_list[idx].th_id;
        tg->lock_list[idx].degrees--;
        v.type = PROCESS;
        //删除边
        if (find_edge(u, v)) {
            remove_edge(u, v);
        }
        //设为本线程
        tg->lock_list[idx].th_id = thread_id;

//        printf("获得 lock idx:%d \n", idx);
    }

    pthread_mutex_unlock_f(&tg->mutex);
}
// static void unlock_before();
static void unlock_after(unsigned long int thread_id, unsigned long int lock) {
    pthread_mutex_lock_f(&tg->mutex);
    // printf("unlock_after lock:%d \n", lock);
    int idx = search_lock(lock);
    if(idx != -1){
        //如果入度为0，说明没有别的线程指向该锁，则把这个idx位置置空
        if (tg->lock_list[idx].degrees == 0) {
            tg->lock_list[idx].th_id = 0;
            tg->lock_list[idx].lock_id = 0;
        }
    }

    pthread_mutex_unlock_f(&tg->mutex);
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
    pthread_t self_id = pthread_self(); 
    
    lock_before(self_id, (unsigned long int) mutex);
    pthread_mutex_lock_f(mutex);
    lock_after(self_id, (unsigned long int) mutex);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    pthread_t self_id = pthread_self();

    pthread_mutex_unlock_f(mutex);
    unlock_after(self_id, (unsigned long int) mutex);
}

int init_hook(void)
{
    if(!pthread_mutex_lock_f){
        pthread_mutex_lock_f = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    }
    if(!pthread_mutex_unlock_f){
        pthread_mutex_unlock_f = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    }
    return 0;
}

void detect_deadlock_loop(void)
{
    while(1)
    {
        detect_deadlock();
        sleep(1);
    }
}

void start_check(void)
{
    init_graph();
    pthread_t tid;
    pthread_create(&tid, NULL, (void*)detect_deadlock_loop, NULL);
}