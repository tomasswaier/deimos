#include <stdio.h>
#include <stdlib.h>

#define INFINITY 1000000

typedef struct Edge {
    int dest;
    int weight;
    struct Edge* next;
} Edge;

typedef struct Node {
    int value;
    Edge* edges;
} Node;

typedef struct Graph {
    int size;
    Node* nodes;
} Graph;

int isFirst = 0;
void printSpace() {
    if (isFirst == 0)
        isFirst++;
    else
        printf("\n");
}

Graph* createGraph(int size) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = size;
    graph->nodes = (Node*)calloc(size, sizeof(Node));
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = NULL;
    if(src >dest){
      int temp=dest;
      dest=src;
      src=temp;
    }


    if (graph->nodes[src].edges == NULL) {
        graph->nodes[src].edges = newEdge;
    } else {

        Edge* current = graph->nodes[src].edges;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEdge;
    }
}


void printGraph(Graph* graph) {
    printf("debugovanie:\n");
    for (int i = 0; i < graph->size; i++) {
        printf("printujem %d:", i);
        Edge* current = graph->nodes[i].edges;
        while (current != NULL) {
            printf(" (%d, %d)", current->dest, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

void deleteEdge(Graph* graph, int src, int dest) {
    Edge* current = graph->nodes[src].edges;
    Edge* prev = NULL;
    if(src >dest){
      int temp=dest;
      dest=src;
      src=temp;
    }
    while (current != NULL) {
        if (current->dest == dest) {
            if (prev == NULL) {
                graph->nodes[src].edges = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void updateEdge(Graph* graph, int src, int dest, int newVal) {
    if (src < 0 || src >= graph->size || dest < 0 || dest >= graph->size) {
        printSpace();
        printf("update %d %d failed", src, dest);
        return;
    }
    if(src >dest){
      int temp=dest;
      dest=src;
      src=temp;
    }

    Edge* current = graph->nodes[src].edges;

    while (current != NULL) {
        if (current->dest == dest) {
            current->weight = newVal;
            return;
        }
        current = current->next;
    }

    // If the loop completes without finding the edge, print failure message
    printSpace();
    printf("update %d %d failed", src, dest);
}

void dijkstra(Graph* graph, int startNode, int endNode) {
    int* distances = (int*)malloc(graph->size * sizeof(int));
    int* prev = (int*)malloc(graph->size * sizeof(int));
    int* visited = (int*)calloc(graph->size, sizeof(int));
    if(startNode >endNode){
      int temp=endNode;
      endNode=startNode;
      startNode=temp;
    }

    for (int i = 0; i < graph->size; i++) {
        distances[i] = INFINITY;
        prev[i] = -1;
    }
    distances[startNode] = 0;

    for (int count = 0; count < graph->size - 1; count++) {
        int minDistance = INFINITY;
        int minNode = -1;

        for (int i = 0; i < graph->size; i++) {
            if (!visited[i] && distances[i] < minDistance) {
                minDistance = distances[i];
                minNode = i;
            }
        }

        if (minNode == -1) {
            // No path exists between startNode and endNode
            printSpace();
            printf("search %d %d failede", startNode, endNode);
            free(distances);
            free(prev);
            free(visited);
            return;
        }

        visited[minNode] = 1;

        Edge* currentEdge = graph->nodes[minNode].edges;
        while (currentEdge != NULL) {
            int neighbor = currentEdge->dest;
            int weight = currentEdge->weight;
            int newDistance = distances[minNode] + weight;
            if (!visited[neighbor] && newDistance < distances[neighbor]) {
                distances[neighbor] = newDistance;
                prev[neighbor] = minNode;
            }
            currentEdge = currentEdge->next;
        }
    }

    // Path exists, reconstruct it
    int current = endNode;
    int* path = (int*)malloc(graph->size * sizeof(int));
    int pathLength = 0;
    while (current != startNode && prev[current] != -1) {
        path[pathLength++] = current;
        current = prev[current];
    }
    path[pathLength++] = startNode;

    printSpace();
    printf("%d: [", distances[endNode]);
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0)
            printf(", ");
    }
    printf("]");

    free(distances);
    free(prev);
    free(visited);
    free(path);
}
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->size; i++) {
        Edge* current = graph->nodes[i].edges;
        while (current != NULL) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->nodes);
    free(graph);
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    getchar();

    Graph* graph = createGraph(N);

    for (int i = 0; i < M; i++) {
        int src, dest, weight;
        scanf("(%d, %d, %d)\n", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    char mode = 'k';
    while ((mode = getchar()) != EOF) {
        switch (mode) {
            case 's': {
                int startNode, endNode;
                scanf("%d %d", &startNode, &endNode);
                dijkstra(graph, startNode, endNode);
                break;
            }
            case 'g': {
                printGraph(graph);
                break;
            }
            case 'i': {
                int src, dest, weight;
                scanf("%d %d %d", &src, &dest, &weight);
                if ((src < N && dest < N && src != dest) && !(src < 0 || dest < 0)) {
                    addEdge(graph, src, dest, weight);
                } else {
                    printSpace();
                    printf("insert %d %d failed", src, dest);
                }
                break;
            }
            case 'd': {
                int src, dest;
                scanf("%d %d", &src, &dest);
                if (src < N && dest < N) {
                    deleteEdge(graph, src, dest);
                } else {
                }
                break;
            }
            case 'u': {
                int src, dest, newVal;
                scanf("%d %d %d", &src, &dest, &newVal);
                if (src < N && dest < N) {
                    updateEdge(graph, src, dest, newVal);
                } else {
                }
                break;
            }
            case '\n':
                break;
        }
    }

    freeGraph(graph);
    return 0;
}
