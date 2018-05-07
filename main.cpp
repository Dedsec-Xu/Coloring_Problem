#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define status int
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define Maxiter 1000000
#define DESCOLOR 50
#define maxdelt 1000000

using namespace std;

typedef struct edgeNode
{
    int name;
    struct edgeNode *next;
} EdgeNode, *EdgeNodePtr;

typedef struct headNode
{
    int degree;
    EdgeNodePtr Edge;
} HeadNode, *HeadNodePtr;

//typedef struct TabuList{
//int ;
//EdgeNodePtr Edge;
//}HeadNode, *HeadNodePtr;

clock_t tstart, tend;
char debug;

void insertEdge(HeadNodePtr H, int start, int target);
void insertEdge2(HeadNodePtr H, int start, int target);

void TabuSearch(
    HeadNodePtr H,
    int &delta,
    int sol[],
    int Tabulist[][DESCOLOR],
    int Adjacent_Color_Table[][DESCOLOR],
    int size_node, int size_edge, FILE *log);

void FindMove(int &u, int &vi, int &vj, int &delt, int delta, int min_f, int iter, int Tabulist[][DESCOLOR], int Adjacent_Color_Table[][DESCOLOR], int sol[], int size_node);

void MakeMove(HeadNodePtr H, int u, int vi, int vj, int delt, int sol[], int &delta, int &min_f, int iter, int Tabulist[][DESCOLOR], int Adjacent_Color_Table[][DESCOLOR]);

int main()
{
    FILE *graphdata;          //graphdata in file
    FILE *log;                //log file
    char path[100];           //file directory
    int size_node, size_edge; //size of the graph
    int n_1, n_2;             //two datas
    int i, j, tt;
    int color;
    int TabuTenure;
    int max = Maxiter;
    EdgeNodePtr Operator;
    EdgeNodePtr head;

    unsigned int seed = time(NULL);
    srand(seed);

    //*****************************
    //      fileopen
    //*****************************

    //printf("File Directory:");
    //scanf("%s",path);
    graphdata = fopen("K:\\Documents\\Coloring_Problem\\instances\\DSJC500.9.col", "r");
    log = fopen("log.txt", "w");
    color = DESCOLOR;

    fscanf(graphdata, "%d %d", &size_node, &size_edge);

    fprintf(log, "opened file :\n%d nodes and %d edges.\nRandom seed: %u\n", path, size_node, size_edge, seed);
    fprintf(log, "color: %d\nMaxiter: %d\n", color, max);
    printf("opened file :\n%d nodes and %d edges.\nRandom seed: %u\n", path, size_node, size_edge, seed);
    printf("color: %d\nMaxiter: %d\n", color, max);
    printf("DEBUG?(y):");
    scanf("%c", &debug);

    //int sol[5] = {0, 0, 1, 2, 2};
    int sol[size_node];
    int Tabulist[size_node][DESCOLOR];
    int Adjacent_Color_Table[size_node][DESCOLOR];
    int delta = 0;

    //*****************************
    //      prepare head node
    //*****************************
    HeadNodePtr H = (HeadNodePtr)malloc(size_node * sizeof(HeadNode));
    HeadNodePtr H2 = (HeadNodePtr)malloc(size_node * sizeof(HeadNode));

    for (i = 0; i < size_node; i++)
    {
        (H + i)->degree = 0;
        (H + i)->Edge = NULL;
        (H2 + i)->degree = 0;
        (H2 + i)->Edge = NULL;
        sol[i] = rand() % color; //color form 0 to color - -1

        printf("sol[%d] = %d\n", i, sol[i]);

        for (j = 0; j < color; j++)
        {
            Tabulist[i][j] = 0;
            Adjacent_Color_Table[i][j] = 0;
        }
    }

    //*****************************
    //      create graph
    //*****************************

    while (!feof(graphdata))
    {
        fscanf(graphdata, "%d%d", &n_1, &n_2);
        //printf("%d %d\n", n_1, n_2);
        // printf("%d\t%d\n", n_1, n_2);
        insertEdge(H, n_1, n_2);

        insertEdge2(H2, n_1, n_2);
    }

    //*****************************
    //      initialize Adjacent_Color_Table
    //*****************************
    int arc;
    int thiscolor;

    for (i = 0; i < size_node; i++)
    {
        Operator = (H + i)->Edge;
        //printf("\n%d",size_node);
        if (Operator != NULL)
        {
            for (; Operator != NULL;)
            {
                arc = Operator->name;
                thiscolor = sol[arc];
                Adjacent_Color_Table[i][thiscolor] += 1;
                //printf("\n\nAdjacent_Color_Table[%d][%d] = %d\n", i, thiscolor, Adjacent_Color_Table [i][thiscolor]);
                //printf("Adjacent_Color_Table [%d][sol[%d]] = %d;\n", i, arc, Adjacent_Color_Table [i][thiscolor]);
                //printf("%d ==== %d\n", i , Operator->name);
                if (sol[i] == thiscolor)
                {
                    delta += 1;
                    printf("+1\n");
                }

                Operator = Operator->next;
            }
        }
    }
    delta /= 2;

    /*for(i = 0;i < size_node; i++)
    {
        printf("\n");
        for(j = 0; j < DESCOLOR; j++)
        {
            printf("%d\t", Adjacent_Color_Table [i][j]);

        }

    }*/

    printf("\n%d\n\n\n\n\n\n\nrunning\n", delta);

    tstart = clock();

    TabuSearch(H, delta, sol, Tabulist, Adjacent_Color_Table, size_node, size_edge, log);

    int soll = 1;
    for (i = 0; i < size_node; i++)
    {
        Operator = (H2 + i)->Edge;
        //printf("\n%d",size_node);
        for (; Operator != NULL;)
        {
            soll *= ((sol[Operator->name] == sol[i]) ? 0 : 1);
            //printf("Adjacent_Color_Table [%d][sol[%d]] = %d;\n", i, Operator->name,Adjacent_Color_Table [i][sol[Operator->name]]);
            Operator = Operator->next;
        }
    }
    if (soll == 1)
    {
        printf("\nno conflict\n");
        fprintf(log, "\nno conflict\n");
    }
    else
    {
        printf("\nconflict!!!1\n");
        fprintf(log, "\nconflict!!!\n");
    }
    printf("\ntime = %f\nEnter to quit:", (double)tend - tstart / CLK_TCK);
    fprintf(log, "\ntime = %f", (double)tend - tstart / CLK_TCK);

    fclose(graphdata);
    fclose(log);
    getchar();
    return 0;
}

void insertEdge(HeadNodePtr H, int start, int target)
{
    EdgeNodePtr Operator;

    (H + start - 1)->degree += 1;
    if ((H + start - 1)->Edge == NULL)
    {
        (H + start - 1)->Edge = (EdgeNodePtr)malloc(sizeof(EdgeNode));
        Operator = (H + start - 1)->Edge;
    }
    else
    {
        for (Operator = (H + start - 1)->Edge; Operator->next != NULL; Operator = Operator->next)
            ;
        Operator->next = (EdgeNodePtr)malloc(sizeof(EdgeNode));
        Operator = Operator->next;
    }

    Operator->name = target - 1;
    Operator->next = NULL;
    //printf("%d ==== %d\n", target , start);
    //printf("%d %d\n", start - 1, Operator->name);

    (H + target - 1)->degree += 1;
    if ((H + target - 1)->Edge == NULL)
    {
        (H + target - 1)->Edge = (EdgeNodePtr)malloc(sizeof(EdgeNode));
        Operator = (H + target - 1)->Edge;
    }
    else
    {
        for (Operator = (H + target - 1)->Edge; Operator->next != NULL; Operator = Operator->next)
            ;
        Operator->next = (EdgeNodePtr)malloc(sizeof(EdgeNode));
        Operator = Operator->next;
    }

    Operator->name = start - 1;
    Operator->next = NULL;
}

void insertEdge2(HeadNodePtr H, int start, int target)
{
    EdgeNodePtr Operator;
    int i;

    (H + start - 1)->degree += 1;
    if ((H + start - 1)->Edge == NULL)
    {
        (H + start - 1)->Edge = (EdgeNodePtr)malloc(sizeof(EdgeNode));
        //printf("start %d LOC = %x\n", start - 1, (H + start - 1)->Edge);
        Operator = (H + start - 1)->Edge;
        Operator->name = target - 1;
        Operator->next = NULL;
    }
    else
    {
        for (Operator = (H + start - 1)->Edge; Operator->next != NULL; Operator = Operator->next)
        {
            i++;
        }
        Operator->next = (EdgeNodePtr)malloc(sizeof(EdgeNode));
        Operator = Operator->next;
        Operator->name = target - 1;
        Operator->next = NULL;
    }
}

void TabuSearch(
    HeadNodePtr H,
    int &delta,
    int sol[],
    int Tabulist[][DESCOLOR],
    int Adjacent_Color_Table[][DESCOLOR],
    int size_node, int size_edge, FILE *log)
{
    int iter, u, vi, vj, delt, i;
    iter = 0;
    int min_f = delta;
    for (; delta != 0; iter++)
    {
        FindMove(u, vi, vj, delt, delta, min_f, iter,
                 Tabulist,
                 Adjacent_Color_Table,
                 sol,
                 size_node);
        MakeMove(H, u, vi, vj, delt, sol, delta, min_f, iter, Tabulist, Adjacent_Color_Table);
    }
    tend = clock();
    for (i = 0; i < size_node; i++)
    {

        printf("sol[%d] = %d\n", i, sol[i]);
        fprintf(log, "sol[%d] = %d\n", i, sol[i]);
    }
}

void FindMove(int &u, int &vi, int &vj, int &delt, int delta, int min_f, int iter, int Tabulist[][DESCOLOR], int Adjacent_Color_Table[][DESCOLOR], int sol[], int size_node)
{
    int i, k, vit, vjt;
    int prevdelt = maxdelt;
    int prevdeltt = maxdelt;
    u = -1;
    int ut = -1;
    delt = maxdelt + 1;
    srand(time(0));
    int c = rand();
    int samed = 1, samedt = 1;
    for (i = 0; i < size_node; i++)
    {
        if (Adjacent_Color_Table[i][sol[i]] > 0)
        {
            for (k = 0; k < DESCOLOR; k++)
            {

                if (k != sol[i])
                {
                    delt = Adjacent_Color_Table[i][k] - Adjacent_Color_Table[i][sol[i]]; //calculate delt value of the move <i, sol[i], k>
                    //printf("delt:%d = %d[%d][%d] - %d[%d][%d]\n", delt, Adjacent_Color_Table[i][k], i, k, Adjacent_Color_Table[i][sol[i]], i, sol[i]);
                    if (Tabulist[i][k] > iter)
                    {
                        if (delt < prevdeltt)
                        {
                            ut = i;
                            vit = sol[i];
                            vjt = k;
                            prevdeltt = delt;
                            //update the tabu best move;
                            //printf("update the tabu best move   %d\n",delt);
                            samedt = 1;
                        }
                        if (delt == prevdeltt)
                        {
                            samedt++;
                            if (rand() % samedt == 0)
                            {
                                ut = i;
                                vit = sol[i];
                                vjt = k;
                                prevdeltt = delt;
                                //update the tabu best move;
                                //printf("update the tabu best move   %d\n",delt);
                            }
                        }
                    }
                    else
                    {
                        if (delt < prevdelt)
                        {
                            u = i;
                            vi = sol[i];
                            vj = k;
                            prevdelt = delt;
                            //update the non-tabu best move;
                            //printf("update the non-tabu best move; %d\n",delt);
                            samed = 1;
                        }
                        if (delt == prevdeltt)
                        {
                            samed++;
                            if (rand() % samed == 0)
                            {
                                u = i;
                                vi = sol[i];
                                vj = k;
                                prevdelt = delt;
                                //update the tabu best move;
                                //printf("update the tabu best move   %d\n",delt);
                            }
                        }
                    }
                }
            }
        }
    }
    if ((u == -1) && (ut == -1))
    {

        u = rand() % size_node;

        vj = (sol[u] + rand() % (DESCOLOR - 1) + 1) % DESCOLOR;
        //printf("rand");
    }
    else
    {
        if (prevdeltt + delta < min_f)
        {
            u = ut;
            vi = sol[u];
            vj = vjt;
            //printf("tabu");
        }
    }
    //printf("%d %d %d\n", u, vi, vj);
    vi = sol[u];
    delt = Adjacent_Color_Table[u][vj] - Adjacent_Color_Table[u][vi];
    //printf("delt:%d = %d[%d][%d] - %d[%d][%d]\n", delt, Adjacent_Color_Table[u][vj], u, vj, Adjacent_Color_Table[u][vi], u, vi);
    return;
}

void MakeMove(HeadNodePtr H, int u, int vi, int vj, int delt, int sol[], int &delta, int &min_f, int iter, int Tabulist[][DESCOLOR], int Adjacent_Color_Table[][DESCOLOR])
{
    EdgeNodePtr Operator;
    sol[u] = vj;
    delta += delt;
    if((debug == 'y')||(debug == 'Y') )printf("%d    \r", delta);
    if (delta < min_f)
    {
        if((debug == 'y')||(debug == 'Y') )printf("%d\n", delta);
        min_f = delta;
    }
    //printf("delta + %d = %d\n", delt, delta);
    Tabulist[u][vi] = iter + delta + rand() % 10;

    EdgeNodePtr tempPtr = (H + u)->Edge;
    while (tempPtr != NULL)
    {
        --(Adjacent_Color_Table[tempPtr->name][vi]);
        ++(Adjacent_Color_Table[tempPtr->name][vj]);
        //printf("\n\nAdjacent_Color_Table[%d][%d] = %d\n", tempPtr->name, vi, Adjacent_Color_Table[tempPtr->name][vi]);

        //printf("Adjacent_Color_Table[%d][%d] = %d\n", tempPtr->name, vj, Adjacent_Color_Table[tempPtr->name][vj]);
        tempPtr = tempPtr->next;
    }
}
