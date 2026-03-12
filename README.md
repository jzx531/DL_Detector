# DL_Detector 死锁探查器



通过如图所示的有向图数据结构进行检测:
```mermaid
classDiagram
    %% 定义样式
    class task_graph {
        <<struct>>
        vertex_t vertex_list[MAX]
        int vertex_num
        pair_t lock_list[MAX]
        int lock_num
        pthread_mutex_t mutex
        int path[MAX+1]
        int visited[MAX]
        int k
        int deadlock
    }

    class vertex_t {
        <<struct>>
        pair_t pair
        vertex_t* next
    }

    class pair_t {
        <<struct>>
        unsigned long int th_id
        enum Type type
        unsigned long int lock_id
        int degrees
    }

    %% 关系描述
    task_graph "1" *-- "MAX" vertex_t : 包含 (vertex_list)
    task_graph "1" *-- "MAX" pair_t : 包含 (lock_list)
    
    vertex_t "1" --> "0..1" pair_t : 拥有 (pair)
    vertex_t "1" --> "0..*" vertex_t : 指向 (next / 邻接表)

    %% 注释说明
    note for task_graph "全局图结构\n维护顶点数组、锁表、\nDFS状态(path, visited)"
    note for vertex_t "图的节点\n代表一个线程或资源\nnext指针构成链表(边)"
    note for pair_t "节点数据\n存储线程ID、锁ID、类型"
```

通过dfs算法进行变量如果遍历某一个邻接列表时，发现已经遍历过该邻接列表，则说明出现了死锁。

通过不断遍历每一个邻接列表来检测死锁。

![alt text](graph_detect.png)

启动检测线程，每秒检测一次是否发生死锁

运行结果:

```bash
./output/main
start_check
-----------detect deadlock----------
No deadlock
----------------------------------
thread_routine 1 : 132766547043904
thread_routine 2 : 132766538651200
thread_routine 3 : 132766530258496
thread_routine 4 : 132766521865792
-----------detect deadlock----------
No deadlock
----------------------------------
-----------detect deadlock----------
Deadlock Path:
509597248 --> 501204544 --> 492811840 --> 517989952 --> 509597248
----------------------------------
-----------detect deadlock----------
Deadlock Path:
509597248 --> 501204544 --> 492811840 --> 517989952 --> 509597248

```