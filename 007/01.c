#include <stdio.h>
#include <stdlib.h>

#define INFINITY 1000000

typedef struct Node {
  int position;
  int value;
  struct Node *next;
} Node;

typedef struct Graph {
  int size;
  Node **nodes;
  int **adjacency;
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
  graph->adjacency = (int **)malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++) {
    graph->nodes[i] = NULL;
    graph->adjacency[i] = (int *)calloc(size, sizeof(int));
  }
  return graph;
}

void addEdge(Graph *graph, int a, int b, int val) {
  if (graph->adjacency[a][b] && graph->adjacency[a][b] != 0 || a == b ||
      a < 0 || b < 0) {
    printSpace();
    printf("insert %d %d failed", a, b);
    return;
  }
  int copya = a;
  int copyb = b;
  if (b < a) {
    a = b;
    b = copya;
  }
  Node *current = graph->nodes[a];
  while (current != NULL) {
    if (current->position == b) {
      printSpace();
      printf("insert %d %d failed", copya,copyb);
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
  return;
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
  if (!graph->adjacency[a][b] || graph->adjacency[a][b] == 0) {
    printSpace();
    printf("delete %d %d failed", a, b);
    return;
  }
  if(a>b){
    int temp=a;
    a=b;
    b=temp;
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
  return;
}

void updateEdge(Graph *graph, int a, int b, int newVal) {
  int copya=a;
  int copyb=b;
  
  if(a>b){
    a=b;
    b=copya;
  }
  if (!graph-> adjacency[a][b] ||graph->adjacency[a][b] + newVal < 0) {
    printSpace();
    printf("update %d %d failed", copya, copyb);
    return;
  }

  Node *current = graph->nodes[a];
  while (current != NULL) {
    if (current->position == b) {
      current->value += newVal;
      break;
    }
    current = current->next;
  }

  graph->adjacency[a][b] += newVal;
  graph->adjacency[b][a] += newVal;
  return;
}

void dijkstra(Graph *graph, int startNode, int endNode) {
  int *distances = (int *)malloc(graph->size * sizeof(int));
  int *prev = (int *)malloc(graph->size * sizeof(int));
  int *visited = (int *)calloc(graph->size, sizeof(int));

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
  int *path = (int *)malloc(graph->size * sizeof(int));
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

void freeGraph(Graph *graph) {
  for (int i = 0; i < graph->size; i++) {
    Node *current = graph->nodes[i];
    while (current != NULL) {
      Node *temp = current;
      current = current->next;
      free(temp);
    }
    free(graph->adjacency[i]);
  }
  free(graph->nodes);
  free(graph->adjacency);
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
    addEdge(graph, temp1, temp2, val);
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
      if ((temp1 < N && temp2 < N && temp1 != temp2 )&& !(temp1 < 0 || temp2 < 0)) {
        addEdge(graph, temp1, temp2, val);
      } else {
        printSpace();
        printf("insert %d %d failed", temp1, temp2);
      }
      break;
    }
    case 'd': {
      int temp1, temp2;
      scanf("%d %d", &temp1, &temp2);
      if (temp1 < N && temp2 < N) {
        deleteEdge(graph,temp1,temp2);
      } else {
      }
      break;
    }
    case 'u': {
      int temp1, temp2, newVal;
      scanf("%d %d %d", &temp1, &temp2, &newVal);
      if (temp1 < N && temp2 < N || temp1 ==temp2) {
        updateEdge(graph, temp1,temp2, newVal);
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

