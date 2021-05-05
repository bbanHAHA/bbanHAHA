#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
int *s;
unsigned short xsub1[3];
// there three variable is used to build the graph's dfs
#define gray -1
#define white 0
#define black 1
int *gcolor;

/*------------------------------------------------------------DLinkList------------------------------------------------------------*/
typedef struct DNode
{
    int id;
    int degree;
    struct DNode *prior, *next;
} DNode, *DLinkList;

typedef struct DegreeNode
{
    DLinkList a;
    int maxdegree;
} DegreeNode, *DegreeList;

void creatDegList(DegreeList l, int d)
{
    l->a = (DNode *)calloc((d + 1), sizeof(DNode));
    for (int i = 0; i <= d; i++)
    {
        l->a[i].degree = i;
        l->a[i].prior = NULL;
        l->a[i].next = NULL;
        l->a[i].id = 0;
    }
    l->maxdegree = d;
}
/*------------------------------------------------------------Graph LinkList------------------------------------------------------------*/
typedef struct ANode
{
    int vertex;
    struct ANode *next;
} ANode, *PANode; //define Edge connected vertex 

typedef struct VNode
{
    int vertex;
    int degree;
    int original_degree;
    int isdelete;
    int color;
    PANode first;
    DLinkList map;
} VNode, *PVNode; //difine vertex 

typedef struct Graph
{
    PVNode p;
    int vexnum;
    int edgenum;
} Graph, *PGraph; // graph

void creatgraph(PGraph g, int v)
{
    g->p = (VNode *)calloc(v, sizeof(VNode));
    for (int i = 0; i < v; i++)
    {
        g->p[i].vertex = i;
        g->p[i].first = NULL;
        g->p[i].degree = 0;
        g->p[i].original_degree = 0;
        g->p[i].isdelete = 0;
        g->p[i].color = 0;
        g->p[i].map = NULL;
    }
    g->vexnum = v;
    g->edgenum = 0;
}
//create  and initialize the graph

void addedge(PGraph g, int from, int to)
{
    PANode a, b;
    int succ = 0;
    b = g->p[from].first;
    if (from != to)
    {
            a = (PANode)malloc(sizeof(ANode));
            a->next = NULL;
            a->vertex = to;
            a->next = g->p[from].first;
            g->p[from].first = a;
            g->p[from].degree++;
            g->p[from].original_degree++;
            //printf("插入---<%d><%d>----------", from, to);
            succ++;
        

            a = (PANode)malloc(sizeof(ANode));
            a->next = NULL;
            a->vertex = from;
            a->next = g->p[to].first;
            g->p[to].first = a;
            g->p[to].degree++;
            g->p[to].original_degree++;
            //printf("---------<%d><%d>---成功\n", to, from);
            succ++;
        
        if (succ == 2)
        {
            g->edgenum++;
        }
    }
    else
    {
        // printf("FALSE\n");
    }
}
/*------------------------------------------------------------create graph with different methods ------------------------------------------------------------*/
void completegraph(PGraph g)
{
    for (int i = 0; i < g->vexnum; i++)
    {
        for (int j = i+1; j < g->vexnum; j++)
        {
            addedge(g, i, j);
        }
    }
}

void out(PGraph g)
{
    printf("%d\t\t# 0th value = Number of vertices\n", g->vexnum);
    int count = 1;
    for (int i = 0; i < g->vexnum; i++)
    {
        PANode a = g->p[i].first;
        if (a != NULL)
        {
            s[i] = count;
        }
        while (a != NULL)
        {

            printf("%d\t\t# %dth value = Vertex %d is adjacent to Vertex %d\n", g->p[i].vertex, count, i, a->vertex);
            a = a->next;
            count++;
        }
    }
    for (int i = 0; i < g->vexnum; i++)
    {
        if (s[i] != 0)
        {
            printf("%d\t\t# %dth value = starting location for vertex %d’s edges\n", s[i], count, i);
            count++;
        }
    }
}

void circle(PGraph g)
{
    for (int i = 0; i < g->vexnum; i++)
    {
        int j = i + 1;
        if (j == (g->vexnum))
        {
            j = 0;
        }
        addedge(g, i, j);
    }
}

int isedge(PGraph g, int from, int to)
{
    PANode a, b;
    b = g->p[from].first;
    while (b)
    {
        if (b->vertex == to)
        {
            //printf("边存在 %d %d \n", from, to);
            return 1;
        }
        b = b->next;
    }
    //printf("边不存在 %d %d \n", from, to);
    return 0;
}
int edgenum(PGraph g, int from)
{
    //method 1 
    return g->p[from].original_degree;
    //method 2 not efficient
    // PANode a, b;
    // b = g->p[from].first;
    // int number = 0;
    // while (b)
    // {
    //     if (b != NULL)
    //     {
    //         number++;
    //     }
    //     b = b->next;
    // }
    // return number;
}

void uniform(PGraph g, int e)
{
    int i;
    int p;
    for (int j = 0; j < e; j++)
    {
        while (1)
        {
            i = rand() % (g->vexnum);
            while (1)
            {
                p = rand() % (g->vexnum);
                if (i != p)
                {
                    break;
                }
            }
            if (isedge(g, i, p) == 0)
            {
                break;
            }
            //sleep(1);
        }
        addedge(g, i, p);
        //sleep(1);
    }
}

int skewvertex(int a)
{
    float x;
    float slope;
    float random = 0;
    int number;
    float v = a;
    int choosed;
    int value = 0;
    float *possib = (float *)calloc(a, sizeof(float));

    x = 2 / (v - 1);
    slope = -(x) / v;

    for (int i = 0; i < v; i++)
    {
        possib[i] = (i + 1) * slope + x;
        random = random + possib[i];
    }
    float count = 0;
    for (int i = 0; i < a; i++)
    {
        possib[i] = count + possib[i] * 1000000;
        count = possib[i];
    }
    possib[a - 1] = possib[a - 1] + 100;

    number = rand() % 1000100;
    for (int j = 0; j < a; j++)
    {
        if (number <= possib[j])
        {
            value = j;
            break;
        }
    }
    return value;
}

void skew(PGraph g, int e)
{
    int first;
    int second;
    int isok;
    for (int i = 0; i < e; i++)
    {
        while (1)
        {
            first = skewvertex(g->vexnum);
            while (1)
            {
                second = skewvertex(g->vexnum);
                if (second != first)
                {
                    break;
                }
            }
            isok = isedge(g, first, second);
            if (isok != 1)
            {
                addedge(g, first, second);
                break;
            }
        }
    }
}

void normal(PGraph g, int e)
{
    int first;
    int second;
    int isok;
    int new;
    int count = 0;
    first = rand() % g->vexnum;
    while (1)
    {

        if (edgenum(g, first) == (g->vexnum - 1))
        {
            while (1)
            {
                first = rand() % g->vexnum;
                if (edgenum(g, first) < (g->vexnum - 1))
                {
                    break;
                }
            }
        }

        while (1)
        {
            while (1)
            {
                second = rand() % g->vexnum;
                if (second != first)
                {
                    break;
                }
            }
            isok = isedge(g, first, second);
            if (isok != 1)
            {
                addedge(g, first, second);
                count++;
                break;
            }
        }
        if (count >= e)
        {
            break;
        }
    }
}

void InsertNextDNode(DegreeList l, int d, int id, PGraph g) //d = degree id = vertex id
{
    DLinkList s;
    s = (DNode *)calloc(1, sizeof(DNode));
    s->next = l->a[d].next;
    s->id = id;
    if (l->a[d].next != NULL)
    {
        l->a[d].next->prior = s;
    }
    s->prior = &(l->a[d]);
    l->a[d].next = s;
    g->p[id].map = s;
}


int RemoveAnyVertex(PGraph g, DegreeList l, int deg)
{

    //delete any vertex that the value of deg
    int id = 0;
    DNode *q = l->a[deg].next;
    id = l->a[deg].next->id;
    l->a[deg].next = q->next;
    if (q->next != NULL)
    {
        q->next->prior = &(l->a[deg]);
    }
    free(q);
    g->p[id].isdelete = -1;
    g->p[id].map = NULL;
    g->p[id].degree = -1;

    //change other connected vertex degree;
    PANode a;
    a = g->p[id].first;
    int record = 0;

    int change_degree = 0;

    while (a != NULL)
    {

        record = (int)a->vertex;

        if (g->p[record].isdelete != -1)
        {
            change_degree = (int)(g->p[record].degree) - 1;
            g->p[record].degree--;

            DLinkList temp = (DNode *)(g->p[record].map);
            temp->prior->next = temp->next;

            if (temp->next != NULL)
            {
                temp->next->prior = temp->prior;
            }

            temp->next = l->a[change_degree].next;

            if (l->a[change_degree].next != NULL)
            {
                l->a[change_degree].next->prior = temp;
            }
            temp->prior = &(l->a[change_degree]);
            l->a[change_degree].next = temp;
        }
        a = a->next;
    }
    return id;
}

int maxdegree(PGraph g)
{
    int max = 0;
    for (int i = 0; i < g->vexnum; i++)
    {
        if (g->p[i].degree >= max)
        {
            max = g->p[i].degree;
        }
    }
    return max;
}
void writeList(PGraph g, DegreeList l)
{
    for (int i = 0; i < g->vexnum; i++)
    {
        InsertNextDNode(l, g->p[i].degree, g->p[i].vertex, g);
    }
}
void outList(DegreeList l)
{
    DLinkList s;
    for (int i = 0; i <= l->maxdegree; i++)
    {
        printf("NOW degree %d contains：", i);
        if (l->a[i].next != NULL)
        {
            s = l->a[i].next;
            while (1)
            {
                printf("%d ", s->id);
                s = s->next;
                if (s == NULL)
                {
                    printf("\n");
                    break;
                }
            }
        }
        else
        {
            printf("None\n");
        }
    }
}
/*------------------------------------------------------------Smallest Last Ordering------------------------------------------------------------*/
int *orderlist;
int *deleteorder;
int record_degree;
int max_degree = 0;
void SmallestLastOrdering(PGraph g, DegreeList l)
{
    record_degree = 0;
    int last_degree = 0;
    int counter = g->vexnum - 1;
    int id = 0;
    int degree = 0;
    int counter1 = 0;
    while (counter1 < (g->vexnum))
    {
        while (l->a[degree].next == NULL)
        {
            degree++;
        }

        id = RemoveAnyVertex(g, l, degree);
         //this line for out put degree for graph
        printf("when deleting vetex %d, it degree is %d\n",id,degree);
        

        if (degree > last_degree)
        {
            record_degree = degree;
        }
        if (degree < last_degree && degree != (last_degree - 1))
        {
            record_degree = degree;
        }

        last_degree = degree;

        if(degree>max_degree){
            max_degree = degree;
        }
        deleteorder[counter1] = id;
        orderlist[counter] = id;
        counter--;
        degree--;
        counter1++;
        if (degree == -1)
        {
            degree = 0;
        }

        // outList(l);
        // printf("--------------------\n");
    }
}

// while(v){
//         while (degree have not vertex)
//         {
//             degree++;
//         }
//         delelte;
//         degree--;
// }

/*------------------------------------------------------------Smallest Original Degree Last------------------------------------------------------------*/

void SmallestOriginalDegreeLast(DegreeList l, PGraph g)
{
    DNode *q;
    int count = g->vexnum - 1;
    for (int i = 0; i <= (l->maxdegree); i++)
    {
        q = l->a[i].next;
        while (q != NULL)
        {
            orderlist[count] = q->id;
            count--;
            q = q->next;
        }
    }
}
/*------------------------------------------------------------  RandomOrdering   ------------------------------------------------------------*/
void RandomOrdering(DegreeList l, PGraph g)
{
    int random_number = 0;
    int counter = 0;
    for (int i = 0; i < g->vexnum; i++)
    {
        while (1)
        {
            random_number = rand() % g->vexnum;
            if (g->p[random_number].isdelete != -1)
            {
                break;
            }
        }
        g->p[random_number].isdelete = -1;
        orderlist[counter] = random_number;
        counter++;
    }
}

/*------------------------------------------------------------dfs------------------------------------------------------------*/

int counter_dfs = 0;
void dfs(Graph g, int i)
{
    PANode p = NULL;
    gcolor[i] = black;
    orderlist[counter_dfs] = i;
    counter_dfs++;
    p = g.p[i].first;
    while (p != NULL)
    {
        if (gcolor[p->vertex] == white)
        {
            dfs(g, p->vertex);
        }
        p = p->next;
    }
}

void DFSTraverse(Graph g)
{
    int i;
    for (i = 0; i < g.vexnum; i++)
    {
        gcolor[i] = white;
    }

    for (i = 0; i < g.vexnum; i++)
    {
        if (gcolor[i] == white)
        {
            dfs(g, i);
        }
    }
}

/*------------------------------------------------------------BFS------------------------------------------------------------*/
//创建用于广度优先遍历的队列
typedef struct QNode
{
    int vertex;
    struct QNode *next;
} QNode, *PQNode;

typedef struct Queue
{
    PQNode front, rear;
} Queue, *PQueue;

//初始化一个空队列
PQueue initQueue()
{
    PQueue q = (PQueue)malloc(sizeof(Queue));
    PQNode head = (PQNode)malloc(sizeof(QNode));
    q->front = q->rear = head;
    head->next = NULL;
    return q;
}

//队尾入队
void enQueue(PQueue q, int i)
{
    PQNode p = (PQNode)calloc(1, sizeof(QNode));
    p->vertex = i;
    p->next = NULL;
    q->rear->next = p;
    q->rear = p;
}
//判断队列是否为空
int isEmpty(PQueue q)
{
    if (q->front == q->rear)
    {

        return 1;
    }
    else
    {
        return 0;
    }
}
//队头出队
int deQueue(PQueue q)
{
    if (isEmpty(q))
    {
        printf("the queue is empty\n");
        exit(-1);
    }

    PQNode p = q->front->next;
    q->front->next = p->next;
    if (p == q->rear)
        q->rear = q->front;
    int data = p->vertex;
    free(p);
    return data;
}

void BFSTraverse(Graph g)
{

    PQueue q = initQueue();
    PANode p;
    int *visited = (int *)calloc((g.vexnum), sizeof(int));
    int v;
    int counter_bfs = 0;
    for (int i = 0; i < g.vexnum; i++)
        visited[i] = -1;
    for (int i = 0; i < g.vexnum; i++)
    {
        if (visited[i] == -1)
        {
            visited[i] = 1;
            orderlist[counter_bfs] = i;
            counter_bfs++;
            enQueue(q, i);
            while (isEmpty(q) == 0)
            {
                v = deQueue(q);
                for (p = g.p[v].first; p != NULL; p = p->next)
                {
                    if (visited[p->vertex] == -1)
                    {
                        orderlist[counter_bfs] = p->vertex;
                        counter_bfs++;
                        visited[p->vertex] = 1;
                        enQueue(q, p->vertex);
                    }
                }
            }
        }
    }
}

void MediumDegreeFirst(PGraph g, DegreeList l)
{
    int *orderlist_1;
    int counter = 0;
    orderlist_1 = (int *)calloc(g->vexnum, sizeof(int));

    DNode *c;

    for (int i = 0; i <= l->maxdegree; i++)
    {
        c = l->a[i].next;
        if (c == NULL)
        {
            continue;
        }
        else
        {
            while (c != NULL)
            {
                orderlist_1[counter] = c->id;
                counter++;
                c = c->next;
            }
        }
    }
    int a = 0;
    int b = 0;
    b = g->vexnum % 2;
    a = g->vexnum / 2;
    int counter_1 = 0;
    if (b > 0)
    {
        orderlist[counter_1] = orderlist_1[a];
        counter_1++;
        int i = 1;
        while (1)
        {
            if (a == (g->vexnum - 1))
            {
                break;
            }
            a = a + 1;
            orderlist[counter_1] = orderlist_1[a];
            counter_1++;
            orderlist[counter_1] = orderlist_1[a - 2 * i];
            counter_1++;
            i++;
        }
    }
    else
    {
        int i = 0;
        orderlist[counter_1] = orderlist_1[a];
        counter_1++;
        orderlist[counter_1] = orderlist_1[a - (2 * i + 1)];
        counter_1++;
        i++;
        while (1)
        {
            if (a == (g->vexnum - 1))
            {
                break;
            }
            a = a + 1;
            orderlist[counter_1] = orderlist_1[a];
            counter_1++;
            orderlist[counter_1] = orderlist_1[a - (2 * i + 1)];
            counter_1++;
            i++;
        }
    }
}

/*------------------------------------------------------------color------------------------------------------------------------*/
void greedcoloring(PGraph g)
{
    int color = 1;
    PANode a;
    for (int i = 0; i < g->vexnum; i++)
    {
        color = 1;
        int coloring_item = orderlist[i];
        a = g->p[coloring_item].first;
        
        while (a != NULL)
        {
            if (g->p[a->vertex].color == color)
            {
                color++;
                a = g->p[coloring_item].first;
            }
            else
            {
                a = a->next;
            }
        }
        g->p[coloring_item].color = color;
    }
}
int isused_sllo;
void outcolor(PGraph g)
{
    int maxcolor = 0;
    int counter = 0;
    for (int i = 0; i < g->vexnum; i++)
    {
        printf(" VERTEX_NUMBER <%d> COLOR_NUMBER <%d> and ORIGINAL_NUMBER is <%d>\n", orderlist[i], g->p[orderlist[i]].color,g->p[orderlist[i]].original_degree);
        counter = counter + g->p[orderlist[i]].original_degree;
        if (g->p[i].color >= maxcolor)
        {
            maxcolor = g->p[i].color;
        }
    }
    printf("total use %d color\n", maxcolor);
    printf("the average original degree is %d\n", (counter/g->vexnum));
    if(isused_sllo == 1){
    printf("the maximum degree when deleted” value for the smallest last ordering is %d\n", max_degree);
    printf("the size of terminal clique = %d\n", (record_degree + 1));
    }
   
}

int main()
{
    srand((unsigned)time(NULL));
    DegreeList l;
    clock_t start_complete,finish_complete;
    double totaltime;
    int v;
    int edge;
    Graph g;
    while (1)
    {
        printf("Please input the Number of vertices. (MAX = 10,000)\n");
        fflush(stdin);
        scanf("%d", &v);
        fflush(stdin);
        if (v > 10000)
        {
            printf("the value is invalid\n");
        }
        else
        {
            break;
        }
    }
    
    while (1)
    {
        printf("Please input the Number of Edges that show conflict between 2 courses. (MAX = 2,000,000)(Complete graph and circle you can input any number)\n");
        fflush(stdin);
        scanf("%d", &edge);
        fflush(stdin);
        if (v > 2000000)
        {
            printf("the value is invalid\n");
        }
        else
        {
            break;
        }
    }
    gcolor = (int *)calloc(v, sizeof(int));
    orderlist = (int *)calloc(v, sizeof(int));
    s = (int *)calloc(v, sizeof(int));
    deleteorder = (int *)calloc(v, sizeof(int));
    creatgraph(&g, v);
    int model;
    while (1)
    {
        printf("Please choose 1 way to creat the graph you can enter number G : 1.COMPLETE | 2.CYCLE | 3.RANDOM (with DIST below)\n");
        fflush(stdin);
        scanf("%d", &model);
        fflush(stdin);
        if (model >= 4 || model <= 0)
        {
            printf("the value is invalid\n");
        }
        else
        {
            break;
        }
    }
    int model_2;
    switch(model)
    {
    case 1:
        //start_complete=clock();
        completegraph(&g);
        //finish_complete=clock();
        break;
    case 2:
        //start_complete=clock();
        circle(&g);
        //finish_complete=clock();
        break;
    case 3:
        while (1)
        {
            
            printf("Which random way you expected: 1. uniform random 2. skew distribution 3. normal\n");
            fflush(stdin);
            scanf("%d", &model_2);
            fflush(stdin);
            if (model_2 >= 4 || model_2 <= 0)
            {
                printf("the value is invalid\n");
            }
            else
            {
                break;
            }
        }
    }

    switch(model_2)
    {
    case 1:
        //start_complete=clock();
        uniform(&g, edge);
        //finish_complete=clock();
        break;
    case 2:
        //start_complete=clock();
        skew(&g, edge);
        //finish_complete=clock();
        break;
    case 3:
        //start_complete=clock();
        normal(&g, edge);
        //finish_complete=clock();
        break;
    }
    out(&g);
    //totaltime=(double)(finish_complete-start_complete)/CLOCKS_PER_SEC;
    //printf("totaltime_for graph=%fs\n",totaltime);
    int max = maxdegree(&g);
    creatDegList(l, max);
    writeList(&g, l);
    outList(l);
    int model_3;
    while (1)
    {
        printf("Which way to ordering the graph you expected: 1. Smallest Last Ordering 2. Smallest Original Degree Last 3. Random Ordering\n4. DFS 5. BFS 6.Medium Degree First\n");
        fflush(stdin);
        scanf("%d", &model_3);
        fflush(stdin);
        if (model_3 >= 7 || model_3 <= 0)
        {
            printf("the value is invalid\n");
        }
        else
        {
            break;
        }
    }
    switch(model_3)
    {
    case 1:
        //start_complete=clock();
        SmallestLastOrdering(&g, l);
        //finish_complete=clock();
        
        isused_sllo = 1;
        break;
    case 2:
        //start_complete=clock();
        SmallestOriginalDegreeLast(l, &g);
        //finish_complete=clock();
        
        break;
    case 3:
        RandomOrdering(l, &g);
        break;
    case 4:
        DFSTraverse(g);
        break;
    case 5:
        BFSTraverse(g);
        break;
    case 6:
        MediumDegreeFirst(&g,l);
        break;
    }

    start_complete=clock();
    greedcoloring(&g);    
    finish_complete=clock();
    
    
    totaltime=(double)(finish_complete-start_complete)/CLOCKS_PER_SEC;
    outcolor(&g);
    printf("totaltime_for graph=%fs\n",totaltime);
    
    if(model_3 == 1){
    printf("the delete order is:");
    for(int i = 0;i< g.vexnum;i++){
        printf("%d ",deleteorder[i]);
    }
    printf("\n");
    }
    printf("the ordering is :");
    for (int i = 0; i < g.vexnum; i++)
    {
        printf("%d ", orderlist[i]);
    }
    printf("\n");
    
    printf("finished\n");
    
    return 1;
}