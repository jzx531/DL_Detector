#include "Diagraph.h"

task_graph *tg=NULL;

vertex_t* create_vertex(pair_t pair)
{
    vertex_t *ver = (vertex_t*)malloc(sizeof(vertex_t));
    ver->pair = pair;
    ver->next = NULL;
    return ver;
}

vertex_t* find_vertex(pair_t pair)
{
    for(int i=0;i<tg->vertex_num;i++)
    {
        if(tg->vertex_list[i].pair.th_id == pair.th_id )
        {
            return &tg->vertex_list[i];
        }
    }
    return NULL;
}

int search_vertex(pair_t pair)
{
    for(int i=0;i<tg->vertex_num;i++)
    {
        if(tg->vertex_list[i].pair.th_id == pair.th_id )
        {
            return i;
        }
    }
    return -1;
}

vertex_t* add_vertex(pair_t pair)
{
    vertex_t *v= find_vertex(pair);
    if(v== NULL){
        // v = create_vertex(pair);
        if(tg->vertex_num < MAX)
        {
            tg->vertex_list[tg->vertex_num].pair = pair;
            tg->vertex_list[tg->vertex_num].next = NULL;
            tg->vertex_num++;
        }
        else
        {
            printf("Error: vertex_num is full\n");
        }
    }
    return v;
}

inline bool find_in_vertex(vertex_t *vertex, pair_t pair)
{
    vertex_t *temp = vertex->next;
    while(temp!= NULL)
    {
        if(temp->pair.th_id == pair.th_id)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}


bool find_edge(pair_t u,pair_t v)
{
    vertex_t *u_vertex = find_vertex(u);
    if(u_vertex == NULL)
    {
        return false;
    }
    return find_in_vertex(u_vertex,v);
}



void add_edge(pair_t u,pair_t v)
{
    vertex_t *u_vertex = add_vertex(u);
    vertex_t *v_vertex = add_vertex(v);
    if(u_vertex->next == NULL)
    {
        u_vertex->next = create_vertex(v);
    }
    else
    {
        vertex_t *temp = u_vertex->next;
        while(temp->next!= NULL)
        {
            temp = temp->next;
            if(temp->pair.th_id == v.th_id)
            {
                return;
            }
        }
        temp->next = create_vertex(v);
    }
    return;
}

// 删除边
void remove_edge(pair_t u,pair_t v)
{
    vertex_t *u_vertex = find_vertex(u);
    vertex_t *v_vertex = find_vertex(v);
    if(u_vertex == NULL || v_vertex == NULL)
    {
        return;
    }
    vertex_t *temp = u_vertex;
    while(temp->next!= NULL)
    {
        if(temp->next->pair.th_id == v.th_id)
        {
            vertex_t *remove = temp->next;
            temp->next = temp->next->next;
            free(remove);
            return;
        }
        temp = temp->next;
    }
}

void init_graph() {
    tg = (task_graph *)malloc(sizeof(task_graph));
    if (!tg) {
        perror("malloc failed");
        exit(1);
    }
    memset(tg, 0, sizeof(task_graph));
    pthread_mutex_init(&tg->mutex, NULL);
    // tg->mutex = PTHREAD_MUTEX_INITIALIZER;
}

/* ******* ******************check cycle****************** ******* */

//打印死锁路径
void print_deadlock(void)
{
    int i=0;
    printf("Deadlock Path:\n");
    for(i=0;i<tg->k-1;i++)
    {
        printf("%d --> ",tg->vertex_list[tg->path[i]].pair.th_id);
    }
    printf("%d\n",tg->vertex_list[tg->path[i]].pair.th_id);
}

void print_locklist(void) {
    int i = 0;

    printf("-----------print_locklist----------\n");
    for (i = 0; i < tg->lock_num; i++) {
        printf("threadid : %ld, lockid: %ld\n", tg->lock_list[i].th_id, tg->lock_list[i].lock_id);
    }
    printf("-----------------------------------\n");
}

int DFS(int idx)
{
    if(idx == -1)
    {
        return 1;
    }
    vertex_t *vertex = &tg->vertex_list[idx];
    //查找到冲突的锁
    if(tg->visited[idx] == 1)
    {
        tg->path[tg->k] = idx;
        tg->k++;
        print_deadlock();
        tg->deadlock = 1;//死锁标志
        return 0;
    }
    tg->visited[idx] = 1;
    tg->path[tg->k] = idx;
    tg->k++;

    while(vertex->next!= NULL)
    {
        vertex_t *next = vertex->next;
        DFS(search_vertex(next->pair));//递归调用
        tg->k--;
        vertex = next;
        // DFS(search_vertex(vertex->next->pair));
        // tg->k--;
        // vertex = vertex->next;
    }
    
    return 1;
}

//判断某个定点是否有环
int check_cycle(int idx)
{
    vertex_t *vertex = &tg->vertex_list[idx];
    tg->k = 0;
    tg->visited[idx] = 1;
    tg->path[tg->k++] = idx;

    while(vertex->next != NULL)//遍历v的所有邻接点
    {
        //重置标记
        int i = 0;
        for (i = 0; i < tg->vertex_num; i++) {
            if (i == idx) continue;
            tg->visited[i] = 0;
        }
        //重置路径
        for (i = 1; i <= MAX; i++) {
            tg->path[i] = -1;
        }
        tg->k = 1;
        //递归调用DFS
        DFS(search_vertex(vertex->next->pair));
        vertex = vertex->next;
    }
}

//检测死锁
void detect_deadlock(void)
{
    printf("-----------detect deadlock----------\n");
    int i = 0;
    tg->deadlock = 0;
    //遍历所有节点
    for(i=0;i<tg->vertex_num;i++)
    {
        check_cycle(i);
        if(tg->deadlock == 1)
        {
            break;
        }
    }
    if(tg->deadlock == 0)
    {
        printf("No deadlock\n");
    }
    printf("----------------------------------\n");
}


int search_lock(unsigned long int lock)
{
    for(int i=0;i<tg->lock_num;i++)
    {
        if(tg->lock_list[i].lock_id == lock)
        {
            return i;
        }
    }
    return -1;
}

int search_empty_lock(unsigned long int lock)
{
    int i = 0;
    for(i=0;i<tg->lock_num;i++)
    {
        if(tg->lock_list[i].degrees == 0 
            && tg->lock_list[i].lock_id == 0 
            && tg->lock_list[i].th_id == 0)
            {
                return i;
            }
    }
    return tg->lock_num;
}

