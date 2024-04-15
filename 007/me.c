#include <stdio.h>
#include <stdlib.h>

typedef struct edge {
  int dest;
  int weight;
  struct edge *next;
} edge;

typedef struct node {
  edge *next;
} node;
typedef struct stack {
  int val;
  struct stack *next;
} stack;
typedef struct PQNode {
  int top;
  int distance;
} PQNode;

typedef struct {
  PQNode *array;
  int size;
} prioQ;

int isFirst = 0;
void printSpace() {
  if (isFirst == 0)
    isFirst++;
  else
    printf("\n");
}
int add(int b, int val, edge **position) {
  while (*position) {
    if ((*position)->dest == b)
      return 0;
    position = &((*position)->next);
  }
  *position = malloc(sizeof(edge));
  (*position)->dest = b;
  (*position)->weight = val;
  (*position)->next = NULL;
  return 1;
}
void addElp(int start, int target, int val, node *graph) {
  int temp = 0;
  int ogStart = start;
  int ogTarget = target;
  if (start != target)
    temp = add(target, val, &graph[start].next);
  if (temp == 0) {
    printSpace();
    printf("insert %d %d failed", ogStart, ogTarget);
  }

  temp = add(start, val, &graph[target].next);
}
int delete2(int start, int target, node *graph) {
  int temp = 0;
  int ogStart = start;
  int ogTarget = target;
  if (start == target) {
    printSpace();
    printf("delete %d %d failed", ogStart, ogTarget);
    return 0;
  }
  edge *first = graph[start].next;
  if (first->dest == target) {
    graph[start].next = first->next;
    free(first);
    return 1;
  }
  edge *second = first;
  while (first) {
    if (first->dest == target) {
      second->next = second->next->next;
      free(first);
      return 1;
    }
    second = first;
    first = first->next;
  }
  printSpace();
  printf("delete %d %d failed", ogStart, ogTarget);
  return 0;
}

void delete (int start, int target, node *graph) {
  int temp = delete2(start, target, graph);
  if (temp)
    temp = delete2(target, start, graph);
}
int update1(int start, int target, int value, node *graph) {
  int ogStart = start;
  int ogTarget = target;
  if (target == start) {
    printSpace();
    printf("update %d %d failed", ogStart, ogTarget);
    return 0;
  }
  edge *read = graph[start].next;
  while (read) {
    if (read->dest == target) {
      if (read->weight + value < 0) {
        printSpace();
        printf("update %d %d failed", ogStart, ogTarget);
        return 0;
      } else {
        read->weight += value;
        return 1;
      }
    }
    read = read->next;
  }
  printSpace();
  printf("update %d %d failed", ogStart, ogTarget);
  return 0;
}

void update(int start, int target, int value, node *graph) {
  int temp = update1(start, target, value, graph);
  if (temp)
    temp = update1(target, start, value, graph);
}
void debug(int N, node *graph) {
  for (int i = 0; i < N; i++) {
    printf("node %d:", i);
    node *temp = &graph[i];
    edge *stemp = temp->next;
    while (stemp) {
      printf("|%d-> %d|", stemp->dest, stemp->weight);
      stemp = stemp->next;
    }
    printf("\n");
  }
}

prioQ *makePrioQ(int num_vertices) {
  prioQ *pq = (prioQ *)malloc(sizeof(prioQ));
  pq->array = (PQNode *)malloc(num_vertices*2* sizeof(PQNode));
  pq->size = 0;
  return pq;
}

void heapify(prioQ *pq, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < pq->size &&
      pq->array[left].distance < pq->array[smallest].distance)
    smallest = left;

  if (right < pq->size &&
      pq->array[right].distance < pq->array[smallest].distance)
    smallest = right;

  if (smallest != idx) {
    PQNode temp = pq->array[smallest];
    pq->array[smallest] = pq->array[idx];
    pq->array[idx] = temp;
    heapify(pq, smallest);
  }
}

void insert(prioQ *pq, int top, int distance) {
  pq->size++;
  int i = pq->size - 1;
  pq->array[i].top = top;
  pq->array[i].distance = distance;

  while (i > 0 && pq->array[(i - 1) / 2].distance > pq->array[i].distance) {
    PQNode temp =pq->array[i];
    pq->array[i] = pq->array[(i - 1) / 2];
    pq->array[(i - 1) / 2] = temp;
    i = (i - 1) / 2;
  }
}

PQNode extractMin(prioQ *pq) {
  PQNode min = pq->array[0];
  pq->array[0] = pq->array[pq->size - 1];
  pq->size--;
  heapify(pq, 0);
  return min;
}
void dijkstra(int start, int target, node *graph, int N) {
  int ogStart = start;
  int ogTarget = target;
  int *distances = malloc(N * sizeof(int));
  int *previous = malloc(N * sizeof(int));
  int *visited = calloc(N, sizeof(int));
  prioQ *queue = makePrioQ(N);

  if (start == target) {
    printSpace();
    printf("search %d %d failed", start, start);
  }

  for (int i = 0; i < N; i++) {
    distances[i] = -1;
    previous[i] = -1;
  }

  distances[start] = 0;
  insert(queue, start, 0);

  while (queue->size != 0) {

    PQNode current = extractMin(queue);
    int toplaner = current.top;
    visited[toplaner] = 1;

    edge *adjacent = graph[toplaner].next;
    while (adjacent != NULL) {
      int dest = adjacent->dest;
      int weight = adjacent->weight;

      if (!visited[dest] &&
          (distances[toplaner] + weight < distances[dest] ||
           distances[dest] == -1)) {
        distances[dest] = distances[toplaner] + weight;
        previous[dest] = toplaner;
        insert(queue, dest, distances[dest]);
      }

      adjacent = adjacent->next;
    }
  }

  if (distances[target] != -1) {
    printSpace();
    printf("%d: [", distances[target]);
    int current = target;
    stack *path = NULL;
    while (current != -1) {
      stack *newNode = malloc(sizeof(stack));
      newNode->val = current;
      newNode->next = path;
      path = newNode;
      current = previous[current];
    }
    while (path != NULL) {
      printf("%d", path->val);
      if (path->next != NULL) {
        printf(", ");
      }
      stack *temp = path;
      path = path->next;
      free(temp);
    }
    printf("]");
  } else {
    printSpace();
    printf("search %d %d failed", ogStart, ogTarget);
  }

  free(distances);
  free(previous);
  free(visited);
  free(queue->array);
  free(queue);
}
void freeing(int N, node *graph) {
  for (int i = 0; i < N; i++) {
    node *temp = &graph[i];
    edge *stemp = temp->next;
    while (stemp) {
      edge *tempo = stemp;
      stemp = stemp->next;
      free(tempo);
    }
  }
  free(graph);
}

int main() {
  int N, M;
  scanf("%d %d", &N, &M);
  getchar();

  node *graph = malloc(N * sizeof(struct node));
  for (int i = 0; i < N; i++) {
    graph[i].next = NULL;
  }
  for (int i = 0; i < M; i++) {
    int start, target, val;
    scanf("(%d, %d, %d)", &start, &target, &val);
    getchar();
    addElp(start, target, val, graph);
  }

  char mode = 'k';
  while ((mode = getchar()) != EOF) {
    switch (mode) {
    case ('g'):
      debug(N, graph);
      break;
    case ('i'): {
      int a, b, val;
      scanf(" %d %d %d", &a, &b, &val);
      addElp(a, b, val, graph);
      break;
    }
    case ('u'): {
      int a, b, val;
      scanf(" %d %d %d", &a, &b, &val);
      update(a, b, val, graph);
      break;
    }
    case ('d'): {
      int a, b;
      scanf(" %d %d", &a, &b);
      delete (a, b, graph);
      break;
    }
    case ('s'): {
      int a, b;
      scanf(" %d %d", &a, &b);
      dijkstra(a, b, graph, N);
      break;
    }
    case ('p'): {
      printf("Not implemented yet\n"); // Placeholder for priority queue testing
      break;
    }
    case ('\n'):
      break;
    }
  }

  freeing(N, graph);
  return 0;
}
