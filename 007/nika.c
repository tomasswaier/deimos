#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 2147483647

typedef struct Node{
    int dest;
    int weight;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} AdjList;

typedef struct {
    int num_vertices;
    AdjList* array;
} Graph;

typedef struct {
    int vertex;
    int distance;
} PQNode;

typedef struct {
    PQNode* array;
    int size;
} PriorityQueue;


Node* createNode(int dest, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int num_vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->array = (AdjList*)malloc(num_vertices * sizeof(AdjList));

    for (int i = 0; i < num_vertices; ++i)
        graph->array[i].head = NULL;

    return graph;
}


int addEdge(Graph* graph, int src, int dest, int weight) {
    if (src == dest || src < 0 || dest < 0 ) {
        return 0;
    }


    Node* current = graph->array[src].head; //loop handling
    while (current != NULL) {
        if (current->dest == dest) {
            return 0;
        }
        current = current->next;
    }

    Node* newNode = createNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;

    return 1;
}

PriorityQueue* createPriorityQueue(int num_vertices) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->array = (PQNode*)malloc(num_vertices*2* sizeof(PQNode));
    pq->size = 0;

    for (int i = 0; i < num_vertices*2; i++) {
        pq->array[i].vertex = -1;
        pq->array[i].distance = INT_MAX;
    }
    return pq;
}

void swap(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(PriorityQueue* pq, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->array[left].distance < pq->array[smallest].distance)
        smallest = left;

    if (right < pq->size && pq->array[right].distance < pq->array[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        swap(&pq->array[smallest], &pq->array[idx]);
        heapify(pq, smallest);
    }
}

void insert(PriorityQueue* pq, int vertex, int distance) {
    pq->size++;
    int i = pq->size - 1;
    pq->array[i].vertex = vertex;
    pq->array[i].distance = distance;

    while (i != 0 && pq->array[(i - 1) / 2].distance > pq->array[i].distance) {
        swap(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

PQNode extractMin(PriorityQueue* pq) {
    PQNode min = pq->array[0];
    pq->array[0] = pq->array[pq->size - 1];
    pq->size--;
    heapify(pq, 0);
    return min;
}

int dijkstra(Graph* graph, int src, int dest, int* newline) {
    int* dist = (int*)malloc(graph->num_vertices * sizeof(int));
    int* parent = (int*)malloc(graph->num_vertices * sizeof(int));
    int* visited = (int*)malloc(graph->num_vertices * sizeof(int));

    for (int i = 0; i < graph->num_vertices; ++i) {
        visited[i] = 0 ;
    }

    for (int i = 0; i < graph->num_vertices; ++i) {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }

    dist[src] = 0;
    PriorityQueue* pq = createPriorityQueue(graph->num_vertices);
    insert(pq, src, 0);

    while (pq->size != 0) {
        int u = extractMin(pq).vertex;
        visited[u] = 1;

        for (Node* node = graph->array[u].head; node; node = node->next) {
            int v = node->dest;
            int weight = node->weight;

            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                insert(pq, v, dist[v]);
            }
        }
    }

    if (dist[dest] == INT_MAX) {
        free(pq->array);
        free(pq);
        free(dist);
        free(parent);
        free(visited);
        return 0;
    }

    int* path = (int*)malloc(graph->num_vertices * sizeof(int));
    int path_length = 0;
    int current_vertex = dest;
    while (current_vertex != src && parent[current_vertex] != -1) {
        path[path_length++] = current_vertex;
        current_vertex = parent[current_vertex];
    }
    path[path_length++] = src;

    if ((*newline)==1){
        printf("\n");
    } else {
        (*newline)++;
    }

    if(dist[dest]==0 && path_length==1){
        printf("search failed");
        free(pq->array);
        free(pq);
        free(path);
        free(dist);
        free(parent);
        free(visited);
        return 1;
    }

    printf("%d: [%d", dist[dest], path[path_length - 1]);
    for (int i = path_length - 2; i >= 0; --i) {
        printf(", %d", path[i]);
    }
    printf("]");

    free(pq->array);
    free(pq);
    free(path);
    free(dist);
    free(parent);
    free(visited);
    return 1;
}

int updateEdge(Graph* graph, int src, int dest, int weight) {
    Node* node = graph->array[src].head;
    int old_weight = -1;
    while (node) {
        if (node->dest == dest) {
            old_weight = node->weight;
            break;
        }
        node = node->next;
    }

    if (old_weight == -1) {
        return 0;
    }

    int new_weight = old_weight + weight;

    if (new_weight < 0) {
        return 0;
    }

    node = graph->array[src].head;
    while (node) {
        if (node->dest == dest) {
            node->weight = new_weight;
            break;
        }
        node = node->next;
    }

    node = graph->array[dest].head;
    while (node) {
        if (node->dest == src) {
            node->weight = new_weight;
            break;
        }
        node = node->next;
    }

    return 1;
}

int deleteEdge(Graph* graph, int src, int dest) {
    int edgeDeleted = 0;

    Node* prev = NULL;
    Node* curr = graph->array[src].head;

    while (curr) {
        if (curr->dest == dest) {
            if (prev)
                prev->next = curr->next;
            else
                graph->array[src].head = curr->next;

            free(curr);
            edgeDeleted = 1;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    prev = NULL;
    curr = graph->array[dest].head;

    while (curr) {
        if (curr->dest == src) {
            if (prev)
                prev->next = curr->next;
            else
                graph->array[dest].head = curr->next;

            free(curr);
            edgeDeleted = 1;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return edgeDeleted;
}

int main() {
    int num_vertices, num_edges;
    int i = 0;
    scanf("%d %d", &num_vertices, &num_edges);

    Graph* graph = createGraph(num_vertices);

    for (int j = 0; j < num_edges; ++j) {
        int src, dest, weight;
        scanf(" (%d, %d, %d)", &src, &dest, &weight);
        if(!addEdge(graph, src, dest, weight)){
            if (i == 0) {
                printf("insert %d %d failed",src,dest);
                i = 1;
            } else {
                printf("\ninsert %d %d failed",src,dest);
            }
        }
    }

    char operation='\0';
    char input[30]; //max input length buffer
    int start=0, end=0, weight=0;
    while (fgets(input, sizeof(input), stdin) != NULL) {
        sscanf(input, " %c", &operation);
        if (operation == 's') {
            if (sscanf(input + 2, "%d %d", &start, &end) != 2) {
                if (i == 0) {
                    printf("search failed");
                    i = 1;
                    continue;
                } else {
                    printf("\nsearch failed");
                    continue;
                }
            }
            if (!dijkstra(graph, start, end,&i)){
                if (i == 0) {
                    printf("search failed");
                    i = 1;
                    continue;
                } else {
                    printf("\nsearch failed");
                    continue;
                }
            }
        } else if (operation == 'i') {
            if (sscanf(input + 2, "%d %d %d", &start, &end, &weight) != 3) {
                if (i == 0) {
                    printf("insert failed");
                    i = 1;
                    continue;
                } else {
                    printf("\ninsert failed");
                    continue;
                }
            }
            if (!addEdge(graph, start, end, weight)){
                if (i == 0) {
                    printf("insert %d %d failed",start,end);
                    i = 1;
                    continue;
                } else {
                    printf("\ninsert %d %d failed",start,end);
                    continue;
                }
            }
        } else if (operation == 'u') {
            if (sscanf(input + 2, "%d %d %d", &start, &end, &weight) != 3) {
                if (i == 0) {
                    printf("update failed");
                    i = 1;
                    continue;
                } else {
                    printf("\nupdate failed");
                    continue;
                }
            }
            if(!updateEdge(graph, start, end, weight)){
                if (i == 0) {
                    printf("update %d %d failed",start,end);
                    i = 1;
                    continue;
                } else {
                    printf("\nupdate %d %d failed",start,end);
                    continue;
                }
            }
        } else if (operation == 'd') {
            if (sscanf(input + 2, "%d %d", &start, &end) != 2) {
                if (i == 0) {
                    printf("delete failed");
                    i = 1;
                    continue;
                } else {
                    printf("\ndelete failed");
                    continue;
                }
            }
            if(!deleteEdge(graph, start, end)){
                if (i == 0) {
                    printf("delete %d %d failed",start,end);
                    i = 1;
                    continue;
                } else {
                    printf("\ndelete %d %d failed",start,end);
                    continue;
                }
            }
        }
    }
    for (int j = 0; j < num_vertices; ++j) { //free graph
        Node* curr = graph->array[j].head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);
    return 0;
}

