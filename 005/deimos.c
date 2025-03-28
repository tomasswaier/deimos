#include <stdio.h>
#include <stdlib.h>

typedef struct tree tree;
struct tree {
  int val;
  char name[100];
  char surname[100];
  int day;
  int month;
  int year;
  int height;
  struct tree *right;
  struct tree *left;
};

void memfree(tree *node){
  if(node==NULL){
    return;
  }
  memfree(node->right);
  memfree(node->left);
 free(node); 
return;

}

void read(tree *node){
  if(node==NULL){
    return;
  }
  read(node->right);
  read(node->left);
  printf("%d\n",node->val);
  return;

}
void write(tree **node,tree *new){
  if(*node==NULL){
    printf("writing\n");
    (*node)=new;
  }
  else if((*node)->val>new->val){

    printf("writing on left\n");
    (*node)->val--;
    write(&(*node)->left,new);
  }
  else if((*node)->val<new->val){
    printf("writing on right\n");
    (*node)->val++;
    write(&(*node)->right,new);
  }
  else{
    printf("first\n");
  }  
    
}
void search(tree *node,int num){
  if(node==NULL){
    printf("invalid input\n");
    return;
  }
  else if(node->val==num){
    printf("%d\n%s %s\n%d.%d.%d\n",node->val,node->name,node->surname,node->day,node->month,node->year);	
    return;
  }
  else if(num>node->val)
      search(node->right,num);
  else if(num<node->val)
      search(node->left,num);
  else{ 
      printf("error");
      return;} 



}
int main() {
  tree *first = NULL;
  char type = 'k';
  while ((type = getchar()) != 'o') {
    switch (type) {
    case ('i'): {
      tree *new=malloc(sizeof(tree));
      if (first==NULL)
	  first=new;
      tree *node=first;
      new->left=NULL;
      new->right=NULL;
      scanf("%d %s %s %d.%d.%d",&new->val,new->name,new->surname,&new->day,&new->month,&new->year);
      printf("%d\n%s %s\n%d.%d.%d\n",new->val,new->name,new->surname,new->day,new->month,new->year);	
      write(&node,new); 
      break;
    }
    case ('s'):{
      int firstNum=0;
      scanf("%d",&firstNum);
      char uwu=getchar();
      if(uwu=='\n'){
	printf("searching one num:%d\n",firstNum);
	search(first,firstNum); 
      }
      else{
	int secondNum=0;
	scanf("%d",&secondNum);
	printf("searching nums:%d to %d\n",firstNum,secondNum);
      }
      break;
      }
    case ('d'):
      printf("hiya i\n");
      break;
    case('r'):
      printf("reading");
      read(first);
      break;
    case ('\n'):
      break;
    default:
      printf("uwu\n");
      break;
    }
  }
  memfree(first);
}
