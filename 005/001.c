#include <stdio.h>
#include <stdlib.h>

typedef struct tree tree;
struct tree {
  int val;
  char name[25];
  char surname[25];
  int day;
  int month;
  int year;
  int height;
  struct tree *right;
  struct tree *left;
};
//first struct . top of the tree bcs im lazy:p
tree *first = NULL;
//height calculator
int vyska(tree *node){
  if(!node)
      return 0;
  return node->height+1;
}
int max(int num1,int num2){
  return (num1>num2?num1:num2);
}
tree* rotateRight(tree **y){

  tree* x=(*y)->left;
  if((*y)==first)
   first=x;

  tree *t2=x->right;
  x->right =(*y);
  (*y)->left =t2;

  (*y)->height= max(vyska((*y)->left), vyska((*y)->right))+1;
  x->height= max(vyska(x->left), vyska(x->right))+1;
  return x;
}
tree* rotateLeft(tree **y){
   
  tree* x=(*y)->right;
  if((*y)==first)
   first=x;
  tree *t2=x->left;
  x->left=(*y);
  (*y)->right=t2;

  (*y)->height= max(vyska((*y)->left), vyska((*y)->right))+1;
  x->height= max(vyska(x->left), vyska(x->right))+1;
  return x;
}
int balanceIt(tree *node){
  if(node==NULL)
      return 0;
  return vyska(node->left)-vyska(node->right);
}
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
//write function for the thingy ....
//just writing and balancing
void write(tree **node,tree *new){
  int balance=0;
  if((*node)==NULL){
    (*node)=new;
    return;
  }
  else if((*node)->val>new->val){
    write(&(*node)->left,new);
  }
  else if((*node)->val<new->val){
    write(&(*node)->right,new);
  }
  else{
    printf("first\n");
    return; 
  } 

  (*node)->height=max(vyska((*node)->left),vyska((*node)->right))+1;

  balance =balanceIt((*node));
  printf("%d %d\n",(*node)->height,balance);
  if( balance >1 && new->val < (*node)->left->val){
    printf("1\n");
    *node=rotateRight(&(*node));
    return;
  }

  if( balance <- 1 && new->val > (*node)->right->val){
    printf("2\n");
     *node=rotateLeft(&(*node));
    return;
  }

  if(balance >1 && new->val >(*node)->left->val){
    printf("3\n");
    (*node)->right=rotateLeft(&(*node)->left); // if it causes weird behavior in results then add the thing to temp instead of node
    *node=rotateRight(&(*node));
    return;
  }
  if(balance <-1 && new->val <(*node)->right->val){
    printf("4\n");
    (*node)->left=rotateLeft(&(*node)->right);
    *node=rotateLeft(&(*node));
    return;
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
      return;} 



}
int main() {
  
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
      new->height=0;
      scanf("%d %s %s %d.%d.%d",&new->val,new->name,new->surname,&new->day,&new->month,&new->year);
     // printf("%d\n%s %s\n%d.%d.%d\n",new->val,new->name,new->surname,new->day,new->month,new->year);	
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
