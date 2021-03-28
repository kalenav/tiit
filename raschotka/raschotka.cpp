#include <iostream>

struct Graph
{
    int** adjMatrix;
    int nodeqty, linkqty;
};

Graph intersect(Graph* graphs, int graphcount)
{
    int intersectionSize = graphs[0].nodeqty;
    for (int currgraph = 1; currgraph < graphcount; currgraph++)
    {
        if (graphs[currgraph].nodeqty < intersectionSize) intersectionSize = graphs[currgraph].nodeqty;
    }
    Graph intersection;
    intersection.nodeqty = intersectionSize;
    intersection.linkqty = 0;
    intersection.adjMatrix = new int* [intersectionSize];
    for (int row = 0; row < intersectionSize; row++)
    {
        intersection.adjMatrix[row] = new int [intersectionSize];
        for (int col = 0; col < intersectionSize; col++)
        {
            int min = graphs[0].adjMatrix[row][col];
            for(int currgraph = 1; currgraph < graphcount; currgraph++)
            {
                min = graphs[currgraph].nodeqty;
                if (graphs[currgraph].adjMatrix[row][col] < min) min = graphs[currgraph].adjMatrix[row][col];
            }
            intersection.adjMatrix[row][col] = min;
            intersection.linkqty += min;
        }
    }
    return intersection;
}

int main()
{
    int graphcount = 0;
    printf("How many graphs will there be? ");
    scanf_s("%d", &graphcount);
    Graph* graphs = new Graph[graphcount];
    for (int currgraph = 0; currgraph < graphcount; currgraph++)
    {
        int currnodecount, currlinkcount;
        printf("Input node count and link count of graph %d: ", currgraph);
        scanf_s("%d %d", &currnodecount, &currlinkcount);
        Graph temp;
        temp.nodeqty = currnodecount;
        temp.linkqty = currlinkcount;
        temp.adjMatrix = new int* [currnodecount];
        for (int row = 0; row < currnodecount; row++)
        {
            temp.adjMatrix[row] = new int [currnodecount];
            for (int col = 0; col < currnodecount; col++)
            {
                temp.adjMatrix[row][col] = 0;
            }
        }
        printf("Specify links in form of \"node1 node2\":\n");
        int node1, node2;
        for (int currlink = 1; currlink <= currlinkcount; currlink++)
        {
            printf("Link %d: ", currlink);
            scanf_s("%d %d", &node1, &node2);
            temp.adjMatrix[node1][node2]++;
            temp.adjMatrix[node2][node1]++;
        }
        graphs[currgraph] = temp;
        for (int i = 0; i < currnodecount; i++)
        {
            for (int j = 0; j < currnodecount; j++)
            {
                printf("%d ", temp.adjMatrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    Graph intersection = intersect(graphs, graphcount);
    printf("The intersection will be described by the following matrix:\n");
    for (int row = 0; row < intersection.nodeqty; row++)
    {
        for (int col = 0; col < intersection.nodeqty; col++)
        {
            printf("%d ", intersection.adjMatrix[row][col]);
        }
        printf("\n");
    }
    return 0;
}