#include <stdio.h>
#include <stdlib.h>
#define ll long long
#define ll_MAX 9123372036854775807
typedef struct PQNode {
  int destination;
  long long distance;
  int source;
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
void array_print(int N, ll **array) {
  for (int i = 0; i < N; i++) {
    printf("|%d|", i);
    for (int j = 0; j < N; j++)
      if (array[i][j] != -1)
        printf("%lld\t", array[i][j]);
    printf("\n");
  }
}

void array_delete(ll ***array, int x, int y) {
  if ((*array)[x][y] < 0 || x == y) {
    printSpace();
    printf("delete %d %d failed", x, y);
    return;
  } else {
    (*array)[x][y] = -1;
    (*array)[y][x] = -1;
  }
}
void array_update(ll ***array, int x, int y, ll value) {
  if ((*array)[x][y] + value < 0 || x == y) {
    printSpace();
    printf("update %d %d failed", x, y);
    return;
  } else {
    (*array)[x][y] += value;
    (*array)[y][x] += value;
  }
}
void array_add(ll ***array, int x, int y, ll value) {
  if ((*array)[x][y] != -1 || value < 0 || x == y) {
    printSpace();
    printf("insert %d %d failed", x, y);
    return;
  } else {
    (*array)[x][y] = value;
    (*array)[y][x] = value;
  }
}

prioQ *makePrioQ(int num_vertices) {
  prioQ *pq = (prioQ *)malloc(sizeof(prioQ));
  pq->array = (PQNode *)malloc(num_vertices * 3 * sizeof(PQNode));
  pq->size = 0;

  for (int i = 0; i < num_vertices * 2; i++) {
    pq->array[i].destination = -1;
    pq->array[i].source = -1;
    pq->array[i].distance = ll_MAX;
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

void insert(prioQ *pq, int destination, ll distance, int source) {
  pq->size++;
  int i = pq->size - 1;
  pq->array[i].destination = destination;
  pq->array[i].distance = distance;
  pq->array[i].source = source;
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

void freePrioQueue(prioQ **queue) {
  free((*queue)->array);
  free(*queue);
}

void flip(int *a, int *b) {
  int temp = (*a);
  (*a) = (*b);
  (*b) = temp;
}

void prim(int N, ll **array, int start) {
  int visited[N];
  int path_vertices[N][2];
  ll key[N];
  int array_position = 0; // position in path_vertices array
  for (int i = 0; i < N; i++) {
    visited[i] = -1;
    path_vertices[i][0] = -1;
    path_vertices[i][1] = -1;
    key[i] = ll_MAX;
  }
  prioQ *queue = makePrioQ(N);
  key[start] = 0;
  insert(queue, start, 0, -1);
  while (queue->size != 0) {
    PQNode current_node = extractMin(queue);
    int node_destination = current_node.destination;
    int node_origin = current_node.source;
    if (visited[node_destination] != -1)
      continue;
    visited[node_destination] = 1;
    path_vertices[array_position][0] =
        node_origin < node_destination ? node_origin : node_destination;
    path_vertices[array_position][1] =
        node_origin > node_destination ? node_origin : node_destination;
    array_position++;
    for (int i = 0; i < N; i++) {
      int weight = array[node_destination][i];
      if (visited[i] == -1 && weight != -1 && weight < key[i]) {
        key[i] = weight;
        insert(queue, i, weight, node_destination);
      }
    }
  }
  unsigned ll result = 0;
  for (int i = 0; i < N; i++) {
    if (visited[i] != -1) {
      result += key[i];
    }
  }
  if (result == 0) {
    printSpace();
    printf("search %d failed", start);
    freePrioQueue(&queue);
    return;
  }

  for (int i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      if (path_vertices[i][0] > path_vertices[j][0] ||
          path_vertices[i][0] == path_vertices[j][0] &&
              path_vertices[i][1] > path_vertices[j][1]) {
        flip(&path_vertices[i][0], &path_vertices[j][0]);
        flip(&path_vertices[i][1], &path_vertices[j][1]);
      }
    }
  }
  printSpace();
  printf("%llu: [", result);
  for (int i = 0; i < N; i++) {
    int temp1 = path_vertices[i][0];
    int temp2 = path_vertices[i][1];
    if (temp1 != -1 && temp2 != -1) {
      printf("(%d, %d)", temp1, temp2);
      if (i != N - 1)
        printf(", ");
    }
  }
  printf("]");
  freePrioQueue(&queue);
}
int main() {
  int N = 0;
  scanf("%d", &N);
  getchar();
  ll **array = malloc(N * sizeof(ll *));
  for (int i = 0; i < N; i++) {
    array[i] = malloc(N * sizeof(ll));
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      array[i][j] = -1;
    }
  }

  char mode = 'k';
  while ((mode = getchar()) != EOF) {
    switch (mode) {
    case ('g'):
      array_print(N, array);
      break;
    case ('('): {
      int a, b;
      ll val;
      scanf("%d, %d, %lld)", &a, &b, &val);
      array_add(&array, a, b, val);
      break;
    }
    case ('i'): {
      int a, b;
      ll val;
      scanf(" %d %d %lld", &a, &b, &val);
      array_add(&array, a, b, val);
      // addElp(a, b, val, graph);
      break;
    }
    case ('u'): {
      int a, b;
      ll val;
      scanf(" %d %d %lld", &a, &b, &val);
      array_update(&array, a, b, val);
      // update(a, b, val, graph);
      break;
    }
    case ('d'): {
      int a, b;
      scanf(" %d %d", &a, &b);
      array_delete(&array, a, b);
      // delete (a, b, graph);
      break;
    }
    case ('s'): {
      int a;
      scanf(" %d", &a);
      prim(N, array, a);
      break;
    }
    case ('\n'):
      break;
    }
  }
  for (int i = 0; i < N; i++) {
    free(array[i]);
  }
  free(array);
  return 0;
}
