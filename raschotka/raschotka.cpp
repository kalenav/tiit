#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

struct Node
{
    int adjacentqty = 0, val;
    Node** adjacent;
};

struct Graph
{
    int nodeqty;
    Node** nodes;
};

bool areAdjacent(Node* node1, Node* node2)
{
    for (int curradj = 0; curradj < node1->adjacentqty; curradj++)
    {
        if (node1->adjacent[curradj] == node2) return true;
    }
    return false;
}

Graph intersectionOf(Graph* graphs, int graphqty)
{
    Graph intersection;
    intersection.nodeqty = graphs[1].nodeqty;
    Graph graphWithFewestNodes = graphs[1];
    for (int currgraph = 2; currgraph <= graphqty; currgraph++)
    {
        if (graphs[currgraph].nodeqty < intersection.nodeqty)
        {
            intersection.nodeqty = graphs[currgraph].nodeqty;
            graphWithFewestNodes = graphs[currgraph];
        }
    }
    intersection.nodes = new Node* [intersection.nodeqty];
    for (int currnode = 1; currnode <= intersection.nodeqty; currnode++)
    {
        intersection.nodes[currnode] = new Node;
    }
    Graph tempgraph;
    for (int currnode = 1; currnode <= intersection.nodeqty; currnode++)
    {
        for (int curradjnode = 1; curradjnode <= intersection.nodeqty; curradjnode++)
        {
            for (int currgraph = 1; currgraph <= graphqty; currgraph++)
            {
                tempgraph = graphs[currgraph];
                if (!(areAdjacent(tempgraph.nodes[currnode], tempgraph.nodes[curradjnode]))) goto nextadjnode;
            }
            intersection.nodes[currnode]->adjacentqty++;
            nextadjnode:;
        }
        intersection.nodes[currnode]->adjacent = new Node* [intersection.nodes[currnode]->adjacentqty];
    }
    int currindex = 0;
    for (int currnode = 1; currnode <= intersection.nodeqty; currnode++)
    {
        for (int curradjnode = 1; curradjnode <= intersection.nodeqty; curradjnode++)
        {
            for (int currgraph = 1; currgraph <= graphqty; currgraph++)
            {
                tempgraph = graphs[currgraph];
                if (!(areAdjacent(tempgraph.nodes[currnode], tempgraph.nodes[curradjnode]))) goto nextadjnode2;
            }
            intersection.nodes[currnode]->adjacent[currindex++] = graphWithFewestNodes.nodes[curradjnode];
            nextadjnode2:;
        }
        currindex = 0;
    }
    return intersection;
}

int main()
{
    int graphcount = 0;
    printf("How many graphs will there be? ");
    scanf("%d", &graphcount);
    Graph* graphs = new Graph[graphcount+1];
    int tempqty;
    for (int currgraph = 1; currgraph <= graphcount; currgraph++)
    {
        printf("\nHow many nodes will there be in graph %d? ", currgraph);
        scanf("%d", &tempqty);
        graphs[currgraph].nodeqty = tempqty;
        graphs[currgraph].nodes = new Node* [tempqty+1];
        for (int currnode = 1; currnode <= tempqty; currnode++)
        {
            graphs[currgraph].nodes[currnode] = new Node;
        }
        for (int currnode = 1; currnode <= tempqty; currnode++)
        {
            printf("Input the nodes adjacent to node %d: first their quantity, then their numbers\n", currnode);
            Node* temp = graphs[currgraph].nodes[currnode];
            temp->val = currnode;
            scanf("%d", &(temp->adjacentqty));
            temp->adjacent = new Node* [temp->adjacentqty];
            int tempnode;
            for (int curr = 0; curr < temp->adjacentqty; curr++) 
            {
                scanf("%d", &tempnode);
                temp->adjacent[curr] = graphs[currgraph].nodes[tempnode];
            }
        }
    }
    Graph intersection = intersectionOf(graphs, graphcount);
    printf("\n\nThe intersection of these graphs will be given by the following adjacency lists:\n");
    for (int currnode = 1; currnode <= intersection.nodeqty; currnode++)
    {
        printf("%d ", intersection.nodes[currnode]->adjacentqty);
        for (int curradjnode = 0; curradjnode < intersection.nodes[currnode]->adjacentqty; curradjnode++)
        {
            printf("%d ", intersection.nodes[currnode]->adjacent[curradjnode]->val);
        }
        printf("\n");
    }
    return 0;
}