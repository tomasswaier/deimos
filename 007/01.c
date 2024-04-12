#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100
#define INFINITY 1000000

typedef struct Node {
    int position;
    int value;
    struct Node *next;
} Node;

typedef struct Graph {
    int size;
    Node **nodes;
    int adjacency[MAX_NODES][MAX_NODES];
} Graph;

int isFirst = 0;
void printSpace() {
  if (isFirst == 0)
    isFirst++;
  else
    printf("\n");
}



Graph *createGraph(int size) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->size = size;
    graph->nodes = (Node **)calloc(size, sizeof(Node *));
    for (int i = 0; i < size; i++) {
        graph->nodes[i] = NULL;
        for (int j = 0; j < size; j++) {
            graph->adjacency[i][j] = 0;
        }
    }
    return graph;
}

void addEdge(Graph *graph, int a, int b, int val) {
    if (graph->adjacency[a][b] != 0) {
        printSpace(); 
        printf("insert %d %d failed", a, b);
        return;
    }

    Node *current = graph->nodes[a];
    while (current != NULL) {
        if (current->position == b) {
            printSpace();
            printf("Edge between %d and %d already exists.", a, b);
            return;
        }
        current = current->next;
    }

    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->position = b;
    newNode->value = val;
    newNode->next = graph->nodes[a];
    graph->nodes[a] = newNode;

    graph->adjacency[a][b] = val;
    graph->adjacency[b][a] = val;
}

void printGraph(Graph *graph) {
    printf("debugovanie:\n");
    for (int i = 0; i < graph->size; i++) {
        Node *current = graph->nodes[i];
        printf("printujem %d:", i);
        while (current != NULL) {
            printf(" (%d, %d)", current->position, current->value);
            current = current->next;
        }
        printf("\n");
    }
}

void deleteEdge(Graph *graph, int a, int b) {
    if (graph->adjacency[a][b] == 0) {
        printSpace();
        printf("delete %d %d failed", a, b);
        return;
    }

    Node *current = graph->nodes[a];
    Node *prev = NULL;

    while (current != NULL) {
        if (current->position == b) {
            if (prev == NULL) {
                graph->nodes[a] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    graph->adjacency[a][b] = 0;
    graph->adjacency[b][a] = 0;
}


void updateEdge(Graph *graph, int a, int b, int newVal) {
    if (graph->adjacency[a][b] + newVal < 0) {
        printSpace();
        printf("update %d %d failed", b, a);
        return;
    }

    Node *current = graph->nodes[a];
    while (current != NULL) {
        if (current->position == b) {
            current->value = newVal;
            break;
        }
        current = current->next;
    }

    graph->adjacency[a][b] += newVal;
    graph->adjacency[b][a] += newVal;
}

void dijkstra(Graph *graph, int startNode, int endNode) {
    int distances[MAX_NODES];
    int prev[MAX_NODES];
    int visited[MAX_NODES] = {0};


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

            break;
        }

        visited[minNode] = 1;


        for (int i = 0; i < graph->size; i++) {
            if (!visited[i] && graph->adjacency[minNode][i] &&
                distances[minNode] != INFINITY &&
                distances[minNode] + graph->adjacency[minNode][i] < distances[i]) {
                distances[i] = distances[minNode] + graph->adjacency[minNode][i];
                prev[i] = minNode;
            }
        }
    }


    int current = endNode;
    int path[MAX_NODES];
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
        if (i > 0) printf(", ");
    }
    printf("]");
}

void freeGraph(Graph *graph) {
    for (int i = 0; i < graph->size; i++) {
        Node *current = graph->nodes[i];
        while (current != NULL) {
            Node *temp = current;
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

    Graph *graph = createGraph(N);

    for (int i = 0; i < M; i++) {
        int temp1, temp2, val;
        scanf("(%d, %d, %d)\n", &temp1, &temp2, &val);
        addEdge(graph, temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1, val);
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
                int temp1, temp2, val;
                scanf("%d %d %d", &temp1, &temp2, &val);
                if (temp1 < N && temp2 < N)
                    addEdge(graph, temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1, val);
                else
                    printf("zly input");
                break;
            }
            case 'd': {
                int temp1, temp2;
                scanf("%d %d", &temp1, &temp2);
                if (temp1 < N && temp2 < N)
                    deleteEdge(graph, temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1);
                else
                    printf("input nieje v hranicia\n");
                break;
            }
            case 'u': {
                int temp1, temp2, newVal;
                scanf("%d %d %d", &temp1, &temp2, &newVal);
                if (temp1 < N && temp2 < N)
                    updateEdge(graph, temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1, newVal);
                else
                    printf("zle input\n");
                break;
            }
            case '\n':
                break;
        }
    }

    freeGraph(graph);
    return 0;
}

