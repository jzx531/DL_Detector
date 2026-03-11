#ifndef DIAGRAPH_H
#define DIAGRAPH_H

#include <stdio.h>
#include <stdbool.h>

#include <pthread.h>
/* ******* ******************Digraph****************** ******* */
#define MAX 1024 //最大线程数

enum Type {
    PROCESS, RESOURCE
};

//锁与线程的信息
typedef struct {
    unsigned long int th_id;
    enum Type type;//线程类型
    unsigned long int lock_id;
    int degrees;//指向该锁的线程数
}pair_t;

//定义图的顶点
typedef struct vertex{
    pair_t pair;
    struct vertex *next;
} vertex_t;

//定义图
typedef struct{
    vertex_t vertex_list[MAX];
    int vertex_num;

    pair_t lock_list[MAX];
    int lock_num;

    pthread_mutex_t mutex;

    //图的路径
    int path[MAX+1];
    int visited[MAX];
    int k;
    int deadlock;
}task_graph;

vertex_t* create_vertex(pair_t pair);



#endif // DIAGRAPH_H

