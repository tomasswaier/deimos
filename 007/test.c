#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 1000000

typedef struct Edge {
  int targetVertex;
  int weight;
  struct Edge *next;
} Edge;

typedef struct Graph {
  int maxVertices;
  Edge **edges;
} Graph;

typedef struct HeapNode {
  int vertex;
  int dist;
} HeapNode;

typedef struct MinHeap {
  int size;
  int capacity;
  int *pos;
  HeapNode **array;
} MinHeap;

void newline(int *isFirst) {
  if (*isFirst == 1) {
    *isFirst = 0;
    return;
  }
  printf("\n");
}

Graph *createGraph(int maxVertices) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->maxVertices = maxVertices;
  graph->edges = (Edge **)calloc(maxVertices, sizeof(Edge *));
  return graph;
}

Edge *createEdge(int targetVertex, int weight) {
  Edge *edge = (Edge *)malloc(sizeof(Edge));
  edge->targetVertex = targetVertex;
  edge->weight = weight;
  edge->next = NULL;
  return edge;
}

MinHeap *createMinHeap(int capacity) {
  MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
  minHeap->pos = (int *)malloc(capacity * sizeof(int));
  minHeap->size = 0;
  minHeap->capacity = capacity;
  minHeap->array = (HeapNode **)malloc(capacity * sizeof(HeapNode *));
  for (int i = 0; i < capacity; ++i) {
    minHeap->array[i] = (HeapNode *)malloc(sizeof(HeapNode));
  }
  return minHeap;
}

void swapHeapNode(HeapNode **a, HeapNode **b) {
  HeapNode *t = *a;
  *a = *b;
  *b = t;
}

void minHeapify(MinHeap *minHeap, int idx) {
  int smallest, left, right;
  smallest = idx;
  left = 2 * idx + 1;
  right = 2 * idx + 2;

  if (left < minHeap->size &&
      minHeap->array[left]->dist < minHeap->array[smallest]->dist) {
    smallest = left;
  }

  if (right < minHeap->size &&
      minHeap->array[right]->dist < minHeap->array[smallest]->dist) {
    smallest = right;
  }

  if (smallest != idx) {
    // Nodes to be swapped in min heap
    minHeap->pos[minHeap->array[smallest]->vertex] = idx;
    minHeap->pos[minHeap->array[idx]->vertex] = smallest;
    swapHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

int isEmpty(MinHeap *minHeap) { return minHeap->size == 0; }

HeapNode *extractMin(MinHeap *minHeap) {
  if (isEmpty(minHeap)) {
    return NULL;
  }

  HeapNode *root = minHeap->array[0];
  HeapNode *lastNode = minHeap->array[minHeap->size - 1];
  minHeap->array[0] = lastNode;

  minHeap->pos[root->vertex] = minHeap->size - 1;
  minHeap->pos[lastNode->vertex] = 0;

  minHeap->size--;
  minHeapify(minHeap, 0);

  return root;
}

void decreaseKey(MinHeap *minHeap, int v, int dist) {
  int i = minHeap->pos[v];
  minHeap->array[i]->dist = dist;

  while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
    minHeap->pos[minHeap->array[i]->vertex] = (i - 1) / 2;
    minHeap->pos[minHeap->array[(i - 1) / 2]->vertex] = i;
    swapHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
    i = (i - 1) / 2;
  }
}

void addEdge(Graph *graph, int src, int dest, int weight, int *isFirst) {
  if (src == dest) {
    newline(isFirst);
    printf("insert %d %d failed", src, dest);
    return;
  }

  Edge *ptr = graph->edges[src];
  while (ptr) {
    if (ptr->targetVertex == dest) {
      newline(isFirst);
      printf("insert %d %d failed", src, dest);
      return;
    }
    ptr = ptr->next;
  }

  Edge *newEdge = createEdge(dest, weight);
  newEdge->next = graph->edges[src];
  graph->edges[src] = newEdge;

  newEdge = createEdge(src, weight);
  newEdge->next = graph->edges[dest];
  graph->edges[dest] = newEdge;
}

void dijkstra(Graph *graph, int src, int dest, int *first) {
  int V = graph->maxVertices;
  MinHeap *minHeap = createMinHeap(V);
  int dist[V], prev[V];

  for (int v = 0; v < V; ++v) {
    dist[v] = INT_MAX;
    prev[v] = -1; // Initialize the predecessor array
    minHeap->array[v]->vertex = v;
    minHeap->array[v]->dist = dist[v];
    minHeap->pos[v] = v;
  }

  minHeap->array[src]->dist = 0;
  dist[src] = 0;
  decreaseKey(minHeap, src, dist[src]);
  minHeap->size = V;

  while (!isEmpty(minHeap)) {
    HeapNode *minHeapNode = extractMin(minHeap);
    int u = minHeapNode->vertex; // The vertex with the minimum distance

    Edge *edge = graph->edges[u];
    if (u == dest)
      break;
    while (edge) {
      int v = edge->targetVertex;
      if (minHeap->pos[v] < minHeap->size && dist[u] != INT_MAX &&
          dist[u] + edge->weight < dist[v]) {
        dist[v] = dist[u] + edge->weight;
        prev[v] = u; // Update the predecessor of v to u
        decreaseKey(minHeap, v, dist[v]);
      }
      edge = edge->next;
    }
  }

  if (dist[dest] == INT_MAX) {
    newline(first);
    printf("search %d %d failed", src, dest);
  } else {
    newline(first);
    int stack[V], top = 0, u = dest;

    // Collect path in stack
    while (u != src && u != -1) {
      stack[top++] = u; // Push each vertex once
      u = prev[u];      // Move to the previous vertex in the path
    }

    // Print path from src to dest
    printf("%d: [%d", dist[dest], src);
    for (int i = top - 1; i >= 0; i--) {
      printf(", %d", stack[i]);
    }
    printf("]");
  }
}

int updateEdge(Graph *graph, int src, int dest, int change, int *first) {
  Edge *ptr = graph->edges[src];
  while (ptr) {
    if (ptr->targetVertex == dest) {
      if (ptr->weight + change >= 0) {
        ptr->weight += change;
        return 1;
      }
    }
    ptr = ptr->next;
  }
  return 0;
}

int removeEdge(Graph *graph, int src, int dest, int *first) {
  Edge **ptr = &graph->edges[src], *temp;
  while (*ptr) {
    if ((*ptr)->targetVertex == dest) {
      temp = *ptr;
      *ptr = (*ptr)->next;
      free(temp);
      return 1;
    }
    ptr = &((*ptr)->next);
  }
  return 0;
}

int main() {
  int N, M, first = 1;
  scanf("%d %d", &N, &M);
  Graph *graph = createGraph(N + 1);

  for (int i = 0; i < M; i++) {
    int u, v, w;
    scanf(" (%d, %d, %d)", &u, &v, &w);
    addEdge(graph, u, v, w, &first);
  }

  char command;
  while (scanf(" %c", &command) != EOF) {
    int u, v, w;
    switch (command) {
    case 'i':
      scanf(" %d %d %d", &u, &v, &w);
      addEdge(graph, u, v, w, &first);
      break;
    case 's':
      scanf(" %d %d", &u, &v);
      dijkstra(graph, u, v, &first);
      break;
    case 'd':
      scanf(" %d %d", &u, &v);
      if (!removeEdge(graph, u, v, &first) ||
          !removeEdge(graph, v, u, &first)) {
        newline(&first);
        printf("delete %d %d failed", u, v);
      }
      break;
    case 'u':
      scanf(" %d %d %d", &u, &v, &w);
      if (!updateEdge(graph, u, v, w, &first) ||
          !updateEdge(graph, v, u, w, &first)) {
        newline(&first);
        printf("update %d %d failed", u, v);
      }
      break;
    }
  }

  return 0;
}
