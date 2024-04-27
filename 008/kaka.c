#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 2147483647

typedef struct Node{
    int dest;
    long long weight;
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
    long long distance;
} PQNode;

typedef struct {
    PQNode* array;
    int size;
} PriorityQueue;


Node* createNode(int dest, long long weight) {
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

void heapify(PriorityQueue* pq, int idx) { //frankenstein heapify
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    PQNode smallestNode = pq->array[smallest];

    if (left < pq->size) { // check if left child exists
        if (pq->array[left].distance < smallestNode.distance)
            smallest = left;
            smallestNode = pq->array[smallest];

        if (right < pq->size && pq->array[right].distance < smallestNode.distance)
            smallest = right;
    }
    else if (right < pq->size && pq->array[right].distance < smallestNode.distance) {
        smallest = right;
    }

    if (smallest != idx) {
        swap(&pq->array[smallest], &pq->array[idx]);
        heapify(pq, smallest);
    }
}

void insert(PriorityQueue* pq, int vertex, long long distance) {
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


int addEdge(Graph* graph, int src, int dest, long long weight) {
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

int updateEdge(Graph* graph, int src, int dest, long long weight) {
    Node* node = graph->array[src].head;
    long long old_weight = -1;
    while (node) {
        if (node->dest == dest) {
            old_weight = node->weight;
            break;
        }
        node = node->next;
    }

    if (old_weight == -1) { //doesnt exist
        return 0;
    }

    long long new_weight = old_weight + weight;

    if (new_weight < 0) { //cant be negative
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

void newline(int *i){
    if (*i == 0){
        (*i)++;
    }else{
        printf("\n");
    }
}

int compare(const void *a, const void *b) { //for qsort
    const int *x = *(const int (*)[2])a;
    const int *y = *(const int (*)[2])b;

    if (x[0] != y[0])
        return x[0] - y[0];

    return x[1] - y[1];
}

int primMST(Graph* graph, int start) {
    int num_vertices = graph->num_vertices;
    int parent[num_vertices];
    long long key[num_vertices];
    int inMST[num_vertices];

    for (int i = 0; i < num_vertices; ++i) {
        key[i] = INT_MAX;
        inMST[i] = 0;
        parent[i] = -1;
    }
    PriorityQueue* pq = createPriorityQueue(num_vertices);

    insert(pq, start, 0);
    key[start] = 0;

    while (pq->size != 0) {
        PQNode minNode = extractMin(pq);
        int u = minNode.vertex;
        inMST[u] = 1;

        Node* current = graph->array[u].head;
        while (current != NULL) {
            int v = current->dest;
            long long weight = current->weight;

            if (inMST[v] == 0 && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                insert(pq, v, key[v]);
            }
            current = current->next;
        }
    }

    unsigned long long sum=0;
    for (int i =0; i < num_vertices; ++i){
        if (parent[i] != -1){
            sum+=key[i];
        }
    }

    if (sum==0){
        free(pq->array);
        free(pq);
        return 0;
    }

    int table[num_vertices][2];

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < 2; j++) {
            table[i][j] = 0;
        }
    }

    for (int i=0; i<num_vertices;i++){
        if (parent[i] != -1){
            if (i<parent[i]) {
                table[i][0]=i;
                table[i][1]=parent[i];
            }
            else {
                table[i][0]=parent[i];
                table[i][1]=i;
            }
        }
    }

    qsort(table, num_vertices, sizeof(table[0]), compare);

    printf("%lld: [",sum);

    for (int i=0;i<num_vertices;i++){
        if (!(table[i][0]==0 && table[i][1]==0)){
            if (i==num_vertices-1){
                printf("(%d, %d)", table[i][0], table[i][1]);
            }else{
                printf("(%d, %d), ", table[i][0], table[i][1]);
            }

        }
    }

    printf("]");

    free(pq->array);
    free(pq);
    return 1;
}

int main() {
    int num_vertices;
    int i = 0;
    scanf("%d", &num_vertices);

    Graph* graph = createGraph(num_vertices);

    while (1) {
        int src, dest;
        long long weight;
        if (scanf(" (%d, %d, %lld)", &src, &dest, &weight) != 3) {
            break; // Break the loop if unable to read an edge
        }
        if (!addEdge(graph, src, dest, weight)) {
            newline(&i);
            printf("insert %d %d failed", src, dest);
        }
    }

    char operation='\0';
    char input[100]; //max input length buffer
    int start=0, end=0;
    long long weight=0;
    while (fgets(input, sizeof(input), stdin) != NULL) {
        sscanf(input, " %c", &operation);
        if (operation == 's') {
            if (sscanf(input + 2, "%d", &start) != 1) {
                newline(&i);
                printf("search failed");
            }
            newline(&i);
            if (!primMST(graph, start)){
                printf("search %d failed",start);
            }
        } else if (operation == 'i') {
            if (sscanf(input + 2, "%d %d %lld", &start, &end, &weight) != 3) {
                newline(&i);
                printf("insert failed");
                continue;
            }
            if (!addEdge(graph, start, end, weight)){
                newline(&i);
                printf("insert %d %d failed",start,end);
                continue;
            }
        } else if (operation == 'u') {
            if (sscanf(input + 2, "%d %d %lld", &start, &end, &weight) != 3) {
                newline(&i);
                printf("update failed");
                continue;
            }
            if(!updateEdge(graph, start, end, weight)){
                newline(&i);
                printf("update %d %d failed",start,end);
                continue;
            }
        } else if (operation == 'd') {
            if (sscanf(input + 2, "%d %d", &start, &end) != 2) {
                newline(&i);
                printf("delete failed");
                continue;
            }
            if(!deleteEdge(graph, start, end)){
                newline(&i);
                printf("delete %d %d failed",start,end);
                continue;
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

