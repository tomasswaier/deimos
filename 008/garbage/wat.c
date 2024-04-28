#include <stdio.h>
#include <stdlib.h>
#define ll long long
#define INT_MAX 2147483647
typedef struct edge {
  int dest;
  long long weight;
  struct edge *next;
} edge;
typedef struct uwu {
  edge *next;
} uwu;
typedef struct stack {
  int val;
  struct stack *next;
} stack;
typedef struct PQNode {
  int top;
  long long distance;
  int bot;
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
int add(int b, ll val, edge **position) {
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
void addElp(int start, int target, ll val, uwu *graph) {
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
int delete2(int start, int target, uwu *graph) {
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

void delete (int start, int target, uwu *graph) {
  int temp = delete2(start, target, graph);
  if (temp)
    temp = delete2(target, start, graph);
}
int update1(int start, int target, ll value, uwu *graph) {
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

void update(int start, int target, int value, uwu *graph) {
  int temp = update1(start, target, value, graph);
  if (temp)
    temp = update1(target, start, value, graph);
}

prioQ *makePrioQ(int num_vertices) {
  prioQ *pq = (prioQ *)malloc(sizeof(prioQ));
  pq->array = (PQNode *)malloc(num_vertices * 8 * sizeof(PQNode));
  pq->size = 0;
  
  for (int i = 0; i < num_vertices*2; i++) {
    pq->array[i].top = -1;
    pq->array[i].bot = -1;
    pq->array[i].distance = INT_MAX;
  }
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

void insert(prioQ *pq, int top, ll distance, int source) {
  pq->size++;
  int i = pq->size - 1;
  pq->array[i].top = top;
  pq->array[i].distance = distance;
  pq->array[i].bot = source;
  while (i > 0 && pq->array[(i - 1) / 2].distance > pq->array[i].distance) {
    PQNode temp = pq->array[i];
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
void flip(int *a, int *b) {
  int temp = (*a);
  (*a) = (*b);
  (*b) = temp;
}
void printSearch(int **ngnl, ll result, int N) {

  for (int i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      if (ngnl[i][0] > ngnl[j][0] ||
          ngnl[i][0] == ngnl[j][0] && ngnl[i][1] > ngnl[j][1]) {
        flip(&ngnl[i][0], &ngnl[j][0]);
        flip(&ngnl[i][1], &ngnl[j][1]);
      }
    }
  }
  printSpace();
  printf("%lld: [", result);
  for (int i = 0; i < N; i++) {
    int temp1 = ngnl[i][0];
    int temp2 = ngnl[i][1];
    if (temp1 != -1 && temp2 != -1) {
      printf("(%d, %d)", temp1, temp2);
      if (i != N - 1)
        printf(", ");
    }
  }
  printf("]");
}
void freePrioQueue(prioQ *queue) {
  free(queue->array);
  free(queue);
}
void prim(int start, uwu *graph, int N) {
  int *distances = malloc(N * sizeof(int));
  int *visited = calloc(N, sizeof(int));
  int **ngnl = malloc(N * sizeof(int *));
  long long *key = malloc(N * sizeof(long long));
  int uwu = 0;
  for (int i = 0; i < N; i++) {
    ngnl[i] = calloc(2, sizeof(int));
  }

  prioQ *queue = makePrioQ(N);
  for (int i = 0; i < N; i++) {
    key[i]=INT_MAX;
    distances[i] = -1;
    ngnl[i][1] = -1;
    ngnl[i][0] = -1;
  }

  distances[start] = 0;
  key[start]=0;
  insert(queue, start, 0, -1);

  while (queue->size != 0) {

    PQNode current = extractMin(queue);
    int toplaner = current.top;
    if (visited[toplaner] != 0)
      continue;

    ngnl[uwu][1] = current.bot > toplaner ? current.bot : toplaner;
    ngnl[uwu][0] = current.bot < toplaner ? current.bot : toplaner;
    uwu++;
    visited[toplaner] = 1;
    //result += current.distance;
    edge *adjacent = graph[toplaner].next;
    while (adjacent != NULL) {
      int dest = adjacent->dest;
      long long weight = adjacent->weight;

      if (visited[dest] == 0 && weight < key[dest]) {
        //printf("inserting %d %d %d\n",dest,weight,toplaner);
        //result += current.distance;
        key[dest]=weight;
        distances[dest] = toplaner;
        insert(queue, dest, weight, toplaner);
      }

      adjacent = adjacent->next;
    }
  }
  
  unsigned long long result = 0;
  for (int i=0;i<N;i++){
    if (distances[i]!=-1){
      result+=key[i];
    }
  }
  if (result == 0) {
    printSpace();
    printf("search %d failed", start);

    free(distances);
    free(visited);
    for (int i = 0; i < N; i++) {
      free(ngnl[i]);
    }
    free(ngnl);

    freePrioQueue(queue);
    return;
  }
  printSearch(ngnl, result, N);
  free(distances);
  free(visited);
  for (int i = 0; i < N; i++) {
    free(ngnl[i]);
  }
  free(ngnl);
  freePrioQueue(queue);
}
void freeing(int N, uwu *graph) {
  for (int i = 0; i < N; i++) {
    uwu *temp = &graph[i];
    edge *stemp = temp->next;
    while (stemp) {
      edge *tempo = stemp;
      stemp = stemp->next;
      free(tempo);
    }
  }
  free(graph);
}
// hihihhihih >,<
int main() {
  int N;
  scanf("%d", &N);
  getchar();

  // omg this is so cute
  uwu *graph = malloc(N * sizeof(struct uwu));
  for (int i = 0; i < N; i++) {
    graph[i].next = NULL;
  }

  char mode = 'k';
  while ((mode = getchar()) != EOF) {
    switch (mode) {
    case ('g'):
      break;
    case ('('): {
      int start, target;
      ll val;
      scanf("%d, %d, %lld)", &start, &target, &val);
      addElp(start, target, val, graph);
      break;
    }
    case ('i'): {
      int a, b;
      ll val;
      scanf(" %d %d %lld", &a, &b, &val);
      addElp(a, b, val, graph);
      break;
    }
    case ('u'): {
      int a, b;
      ll val;
      scanf(" %d %d %lld", &a, &b, &val);
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
      int a;
      scanf(" %d", &a);
      prim(a, graph, N);
      break;
    }
    case ('p'): {
      printf("Not implemented yet\n");
      break;
    }
    case ('\n'):
      break;
    }
  }

  freeing(N, graph);
  return 0;
}
