#include <stdio.h>
#include <stdlib.h>

int **array;
int *writing;
void debug(int N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", array[i][j]);
    }
    printf("\n");
  }
}
void delete (int a, int b) {
  if (array[a][b] != -1)
    array[a][b] = -1;
  else
    printf("delete failed");
}
void insert(int a, int b, int val) {
  if (array[a][b] == -1) {
    array[a][b] = val;
  } else {
    printf("inser faied");
  }
}
void update(int a, int b,int val){

  if (array[a][b] == -1) {
    array[a][b] = val;
  } else {
    printf("udate faied");
  }
}
void search(int start,int end){
  int boolean=0;
  
  while(boolean!=1){
    




  }




}
void setWriting(int N){ //set all values in final tab to -1 = infinity

  for(int i=0;i<N;i++)
    writing[i]=-1;
}
void setup(int N){ // setup the array ( looked ugly in main);

  array = (int **)calloc(N, sizeof(int *));
  for (int i = 0; i < N; i++) {
    array[i] = (int *)calloc(N, sizeof(int));
  }
  writing=malloc(N*sizeof(int));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      array[i][j] = -1;
}
int main() {

  int N, M;
  scanf("%d %d", &N, &M);
  getchar();
  setup(N);
  for (int i = 0; i < M; i++) {
    int temp1, temp2, val;
    scanf("(%d, %d, %d)\n", &temp1, &temp2, &val);

    array[temp1 < temp2 ? temp1 : temp2][temp2 > temp1 ? temp2 : temp1] = val;
  }

  char mode = 'k';
  while ((mode = getchar()) != EOF) {
    switch (mode) {
    case 's': {
      setWriting(N);
      int temp1, temp2;
      scanf("%d %d", &temp1, &temp2);
      if (temp1 < N && temp2 < N)
        search(temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1);

      else
        printf("out of bounds\n");

      break;
    }
    case 'g':
      debug(N);
      break;
    case 'i': {

      int temp1, temp2, val;
      scanf("%d %d %d", &temp1, &temp2, &val);
      if (temp1 < N && temp2 < N)
        insert(temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1,
               val);
      else
        printf("out of bounds");
      break;
    }
    case 'd': {

      int temp1, temp2;
      scanf("%d %d", &temp1, &temp2);
      if (temp1 < N && temp2 < N)
        delete(temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1);
      else
        printf("out of bounds\n");

      break;
    }
    case 'u': {

      int temp1, temp2, val;
      scanf("%d %d %d", &temp1, &temp2, &val);
      if (temp1 < N && temp2 < N)
        update(temp1 < temp2 ? temp1 : temp2, temp2 > temp1 ? temp2 : temp1,
               val);
      else
        printf("out of bounds\n");

      break;
    }
    case '\n':
      break;
    }
  }

  for (int i = 0; i < N; i++) {
    free(array[i]);
  }
  free(array);
  free(writing);

  return 0;
}
