#include <stdio.h>
#include <stdlib.h>
#define max 600000
typedef struct {
  char name[20];
  char surname[20];
  int day;
  int month;
  int year;
  double balance;
} fella;
fella *map[max];
void start_table() {
  for (int i = 0; i < max; i++) {
    map[i] = NULL;
  }
}

int strcmpi(char first[], char second[]) {
  while(*first++!='\0' && *second++!='\0'){
    if (*first > *second)
      return 1;
    else if (*first < *second)
      return -1;
  }
  //add thing if one is ada and then othe one is adamada
  return 0;
}
unsigned int hashIt(char *name, char *surname, int day, int year) {
  int count = 0;
  while (*name++ != '\0') {
    count += name[0] * year + day;
  }
  while (*surname++ != '\0')
    count += surname[0];

  return count;
}
int insert(fella *node) {
  int position = hashIt(node->name, node->surname, node->day, node->year);
  if (map[position] == NULL) {
    map[position] = node;
    return 1;
  } else {
    return 0;
  }
}
void debug() {
  for (int i = 0; i < max; i++) {
    if (map[i] != NULL) {
      printf("%s %s %d.%d.%d %lf\n", map[i]->name, map[i]->surname, map[i]->day,
             map[i]->month, map[i]->year, map[i]->balance);
    }
  }
}

fella *find(char *name, char *surname, int day, int month, int year) {
  int num = hashIt(name, surname, day, year);
  if (map[num]) {
    if (strcmpi(name, map[num]->name) == 0 &&
        strcmpi(surname, map[num]->surname) == 0 && day == map[num]->day &&
        month == map[num]->month && year == map[num]->year)
      return map[num];
  } else {
    return NULL;
  }

  return NULL;
}
int main() {
  start_table();
  char uwu = 'l';
  while ((uwu = getchar()) != EOF) {
    switch (uwu) {
    case ('i'): {

      fella *new = malloc(sizeof(fella));
      scanf("%s %s %d.%d.%d %lf", new->name, new->surname, &new->day,
            &new->month, &new->year, &new->balance);
      insert(new);
      break;
    }
    case ('d'):
      debug();
      break;
    case ('s'):{
      fella *temp=malloc(sizeof(fella));
      scanf("%s %s %d.%d.%d",temp->name,temp->surname,&temp->day,&temp->month,&temp->year);

      fella *new=find(temp->name,temp->surname,temp->day,temp->month,temp->year);
      // search ig
      if(new)
        printf("%s %s %d.%d.%d %lf\n", new->name, new->surname, new->day,
             new->month, new->year, new->balance);
      else{
        printf("nop");
      }
        
      break;
      }
    case ('u'):
      // udjust ?
      break;
    case ('\n'):
      break;
    default:
      printf("cant comprehend");
    }
  }
  for (int i = 0; i < max; i++) {
    if (map[i] != NULL)
      free(map[i]);
  }
}
