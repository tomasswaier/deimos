#include <stdio.h>
#include <stdlib.h>

#define uwu 100

typedef struct person person;
struct person {
  char name[12];
  char surname[12];
  char date[11];
  int bc;
};

person *map[uwu] = {NULL}; // Initialize map to NULL pointers

int ztrcmp(const char *first, const char *second) {
  while (*first && *second) {
    if (*first != *second)
      return (*first > *second) ? 1 : -1;
    first++;
    second++;
  }
  if (*first)
    return 1;
  if (*second)
    return -1;
  return 0;
}

int multistrcmp(person *one, person *two) {
  int result = ztrcmp(one->name, two->name);
  if (result != 0)
    return result;
  result += ztrcmp(one->surname, two->surname);
  if (result != 0)
    return result;
  result += ztrcmp(one->date, two->date);
  return result;
}

int hash(char name[], char surname[], char date[]) {
  long result = 0;
  while (*name != '\0') {
    result += uwu * (long)(*name);
    name++;
  }

  while (*surname != '\0') {
    result += (long)(*surname++);
  }

  while (*date != '\0') {
    result += uwu * (long)(*date++);
  }
  return (int)result % uwu;
}

void debug() {
  for (int i = 0; i < uwu; i++) {
    if (map[i] != NULL) { // Corrected the condition to check if map[i] is not NULL
      printf("%s %s %s %d.%d\n", map[i]->name, map[i]->surname, map[i]->date,map[i]->bc/100, map[i]->bc%100);
    }
  }
}
int search(person *node){
  int position = hash(node->name, node->surname, node->date);
  while (position < uwu) {
    if (map[position]!=NULL &&multistrcmp(node, map[position]) == 0) {
      return map[position]->bc;
    } else {
      position++;
    }
  }
  return -1;

}
int insert(person *node) {
  int position = hash(node->name, node->surname, node->date);
  while (position < uwu) {
    if (map[position] == NULL) { // Check if map[position] is NULL
      map[position] = malloc(sizeof(person)); // Allocate memory for new person
      *map[position] = *node; // Copy the contents of 'node' into the allocated memory
      return 1;
    } else if (multistrcmp(node, map[position]) == 0) {
      printf("This person already exists\n");
      return 0;
    } else {
      position++;
    }
  }
  printf("Failed to insert because of map size\n");
  return 0;
}

int main() {
  char type = 'k';
  while ((type = getchar()) != EOF) {
    switch (type) {
    case ('i'): {
      person *temp = malloc(sizeof(person));
      if (temp == NULL) {
        printf("Memory allocation failed\n");
        break;
      }
      int tempo;
      scanf("%s %s %s %d.%d", temp->name, temp->surname, temp->date, &temp->bc,&tempo);
      temp->bc*=100;
      temp->bc+=tempo;
      int inytemp = insert(temp);
      if(inytemp==0)
        printf("Something went wrong\n");
      free(temp);
      break;
    }
    case ('d'):
      debug();
      break;
    case ('s'):
      person* temp=malloc(sizeof(person));
      scanf("%s %s %s", temp->name, temp->surname, temp->date);
      int tempo=search(temp); 
      if(tempo>=0)
        printf("%d.%d\n",tempo/100,tempo%100);
      else{
        printf("user doesnt exist\n");
      }
      free(temp);
      break;
    case ('\n'):
      break;
    default:
      printf("Didn't understand\n");
    }
  }
  for(int i=0;i<uwu;i++){
    free(map[i]);

  }
  return 0;
}

