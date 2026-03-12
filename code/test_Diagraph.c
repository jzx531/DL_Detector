#include "Diagraph.h"

extern task_graph* tg;
// ================= 测试主函数 =================

int main() {
    printf("=== Deadlock Detector Test ===\n\n");

    // 1. 初始化
    init_graph();

    pair_t v1;
    v1.th_id = 1;
    v1.type = PROCESS;
    add_vertex(v1);

    pair_t v2;
    v2.th_id = 2;
    v2.type = PROCESS;
    add_vertex(v2);

    pair_t v3;
    v3.th_id = 3;
    v3.type = PROCESS;
    add_vertex(v3);

    pair_t v4;
    v4.th_id = 4;
    v4.type = PROCESS;
    add_vertex(v4);


    pair_t v5;
    v5.th_id = 5;
    v5.type = PROCESS;
    add_vertex(v5);


    add_edge(v1, v2);
    add_edge(v2, v3);
    add_edge(v3, v4);
    add_edge(v4, v5);
    add_edge(v3, v1);
    add_edge(v5, v1);

    detect_deadlock();
    printf("\n");

    // 清理
    free(tg);
    return 0;
}

