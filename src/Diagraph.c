#include "Diagraph.h"

static task_graph *tg=NULL;

vertex_t* create_vertex(pair_t pair)
{
    vertex_t *vertex = (vertex_t*)malloc(sizeof(vertex_t));
    vertex->pair = pair;
    vertex->next = NULL;
    return vertex;
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
}

vertex_t* add_vertex(pair_t pair)
{
    vertex_t *v= find_vertex(pair);
    if(v== NULL){
        v = create_vertex(pair);
        if(tg->vertex_num < MAX)
        {
            tg->vertex_list[tg->vertex_num] = *v;
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
        u_vertex->next = v_vertex;
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
        temp->next = v_vertex;
    }
    return;
}

//删除边
void remove_edge(pair_t u,pair_t v)
{
    vertex_t *u_vertex = find_vertex(u);
    // vertex_t *v_vertex = find_vertex(v);
    if(u_vertex == NULL /*|| v_vertex == NULL*/)
    {
        return;
    }
    vertex_t *temp = u_vertex;
    while(temp->next!= NULL)
    {
        if(temp->next->pair.th_id == v.th_id)
        {
            vertex_t *remove = temp->next;
            temp->next = temp->next;
            free(remove);
            return;
        }
        temp = temp->next;
    }
}








