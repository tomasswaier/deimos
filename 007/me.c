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
int delete2 (int start, int target, node *graph) {
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
  int temp= delete2(start,target,graph); 
  if(temp)
      temp= delete2(target,start,graph); 

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
  int temp=update1(start,target,value,graph);
  if(temp)
    temp=update1(target,start,value,graph);


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
void queueAdd(stack **queue, stack *temp, int *tab) {
  if (*queue == NULL || tab[temp->val] <= tab[(*queue)->val]) {

    temp->next = *queue;
    *queue = temp;
    return;
  }

  stack *tempo = *queue;
  while (tempo->next && tab[tempo->next->val] < tab[temp->val]) {
    tempo = tempo->next;
  }
  temp->next = tempo->next;
  tempo->next = temp;
  return;
}
void dijkstra(int start, int target, node *graph, int N) {
  int ogStart = start;
  int ogTarget = target;
  int *distances = malloc(N * sizeof(int));
  int *previous = malloc(N * sizeof(int));
  int *visited = calloc(N, sizeof(int));
  stack *queue = NULL;
  if (start == target) {
    printSpace(); // might be a failurie
    printf("search %d %d failed", start, start);
  }
  for (int i = 0; i < N; i++) {
    distances[i] = -1;
    previous[i] = -1;
  }

  distances[start] = 0;

  stack *temp = malloc(sizeof(stack));
  temp->val = start;
  temp->next = NULL;
  queueAdd(&queue, temp, distances);

  while (queue != NULL) {
    int current = queue->val;
    stack *temp = queue;
    queue = queue->next;
    free(temp);

    visited[current] = 1;

    edge *adjacent = graph[current].next;
    while (adjacent != NULL) {
      int dest = adjacent->dest;
      int weight = adjacent->weight;

      if (distances[current] + weight < distances[dest] ||
          distances[dest] == -1) {
        distances[dest] = distances[current] + weight;
        // Update previous node for path reconstruction
        previous[dest] = current;

        if (!visited[dest]) {
          // If the destination node has not been visited yet, add it to the
          // queue
          stack *newNode = malloc(sizeof(stack));
          newNode->val = dest;
          newNode->next = NULL;
          queueAdd(&queue, newNode, distances);
        }
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
      int a, b, val;
      scanf(" %d %d", &a, &b);
      delete (a, b, graph);
      break;
    }
    case ('s'): {
      int a, b, val;
      scanf(" %d %d", &a, &b);
      dijkstra(a, b, graph, N);
      break;
    }
    case ('p'): {
      stack *fakeQueue = NULL;
      char testingQueue = 'a';
      int *tab = calloc(N, sizeof(int));
      tab[0] = -1;
      // Read and add elements to the priority queue until 'q' is
      // encountered
      while ((testingQueue = getchar()) != EOF) {
        if (testingQueue == '.')
          if (testingQueue == ' ') {
            stack *temp = malloc(sizeof(stack));
            scanf("%d", &temp->val);
            temp->next = NULL;
            queueAdd(&fakeQueue, temp, tab);

            // Print the current state of the priority queue
            stack *tempPrint = fakeQueue;
            while (tempPrint) {
              printf("%d ", tempPrint->val);
              tempPrint = tempPrint->next;
            }
            printf("\n");
          }
      }

      // Free memory allocated for the priority queue
      stack *temp = fakeQueue;
      while (temp) {
        stack *next = temp->next;
        free(temp);
        temp = next;
      }
      free(tab);
      break;
    }
    case ('\n'):
      break;
    }
  }
  freeing(N, graph);
  return 0;
}
