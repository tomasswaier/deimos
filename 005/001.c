#include <stdio.h>
#include <stdlib.h>

typedef struct tree tree;
struct tree {
  int val;
  char name[40];
  char surname[40];
  int day;
  int month;
  int year;
  int height;
  struct tree *right;
  struct tree *left;
};
//first struct . top of the tree bcs im lazy:p
tree *first = NULL;
//I love global variables so yummy uwwuwuwu
int isFirst=1;
//height calculator
int vyska(tree *node){
  if(node==NULL)
      return 0;
  return node->height;
}
int max(int num1,int num2){
  return (num1>num2?num1:num2);
}
tree* rotateRight(tree **y){

  tree* x=(*y)->left;
  tree *t2=x->right;
  x->right =(*y);
  (*y)->left =t2;

  (*y)->height= max(vyska((*y)->left), vyska((*y)->right))+1;
  x->height= max(vyska(x->left), vyska(x->right))+1;
  if(*y==first)
      first=x;
  *y=x;
  return x;
}
tree* rotateLeft(tree **y){
   
  tree* x=(*y)->right;

  tree *t2=x->left;
  x->left=(*y);
  (*y)->right=t2;

  (*y)->height= max(vyska((*y)->left), vyska((*y)->right))+1;
  x->height= max(vyska(x->left), vyska(x->right))+1;
  if(*y==first)
      first=x;
  *y = x;
  

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
  printf("%d %d\n",node->val,node->height);
  return;

}
//write function for the thingy ....
//just writing and balancing
void write(tree **node,tree *new){
  if((*node)==NULL){
    (*node)=new;
    (*node)->right=NULL;
    (*node)->left=NULL;
    return;
  }
  else if((*node)->val>new->val){
    write(&(*node)->left,new);
  }
  else if((*node)->val<new->val){
    write(&(*node)->right,new);
  }
  else{
    return; 
  } 

  (*node)->height=max(vyska((*node)->left),vyska((*node)->right))+1;
  tree *temp;
  int balance =balanceIt((*node));

  if( balance >1 && new->val < (*node)->left->val){
    temp=rotateRight(&(*node));
    if((*node)==first)
      first=temp;


    return;
  }

  if( balance <- 1 && new->val > (*node)->right->val){
    temp=rotateLeft(&(*node));
    if((*node)==first)
      first=temp;
    return;
  }

  if(balance >1 && new->val >(*node)->left->val){
    (*node)->left=rotateLeft(&(*node)->left);
    // if it causes weird behavior in results then add the thing to temp instead of node
    temp=rotateRight(&(*node));
    if(*node==first)
      first=temp;
    return;
  }
  if(balance <-1 && new->val <(*node)->right->val){
    (*node)->right=rotateRight(&(*node)->right);
    temp=rotateLeft(&(*node));
    if((*node)==first)
      first=temp;
    return;
  }
}
tree* search(tree *node,int num){
  if(node==NULL){
    return NULL;
  }
  else if(node->val==num){
    return node;
  }
  else if(num>node->val)
      return search(node->right,num);
  else if(num<node->val)
      return search(node->left,num);



  return NULL;
}


void mulsearch(tree *node, int left, int right) {
    if (node == NULL)
        return;

    if (node->val < left) {
        mulsearch(node->right, left, right);
    } else if (node->val > right) {
        mulsearch(node->left, left, right);
    } else {
        mulsearch(node->left, left, right);
        if (node->val >= left && node->val <= right) {
	  if(isFirst==1)
            printf("%d %s %s %d.%d.%d", node->val, node->name, node->surname, node->day, node->month, node->year);
	  else{
	    printf("\n%d %s %s %d.%d.%d", node->val, node->name, node->surname, node->day, node->month, node->year);
	    isFirst=0;}

        }
        mulsearch(node->right, left, right);
    }
}


//find if the thingy has one or more chilred
//if <2 then move that child up
//else uwu
tree *delete(tree *node,int num){
  if (node==NULL){
    return node;
  }
  if(num<node->val)
    node->left=delete(node->left,num);
  else if(num>node->val)
    node->right=delete(node->right,num);
  else{
    //
    tree *temp;
    if(!node->right ||!node->left ){
      temp=node->left?node->left:node->right;
      if(!temp){
	temp = node;
	node= NULL;
      }
      else if(temp)
	  *node=*temp;
      free(temp);

    }
    else{
      temp=node->right;
      while(temp->left!=NULL)
	temp=temp->left;

      node->val=temp->val;
      node->right=delete(node->right,temp->val);
    }



  }
  if(!node)
    return node;
  
  node->height=max(vyska(node->left),vyska(node->right));
  int balance =balanceIt(node);
  if(balance >1 && balanceIt(node->left)>=0){
    return rotateRight(&node);
  }
  if(balance >1 && balanceIt(node->left)<0){
    node->left=rotateLeft(&node->left);
    return rotateRight(&node);
  }

  if(balance <-1 && balanceIt(node->right)<=0){
    return rotateLeft(&node);
  }

  if(balance <-1 && balanceIt(node->right)>0)
  {
    node->right=rotateRight(&node->right);
    return rotateLeft(&node);
  }
  return node;

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
      new->height=1;
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
	tree *new=search(first,firstNum);
	if(new!=NULL){
	  if(isFirst==0)
	    printf("\n%d %s %s %d.%d.%d",new->val,new->name,new->surname,new->day,new->month,new->year);
	
	  else{
	    printf("%d %s %s %d.%d.%d",new->val,new->name,new->surname,new->day,new->month,new->year);
	    isFirst=0;
	  }
	}
      }
      else{
	int secondNum=0;
	scanf("%d",&secondNum);
	if(firstNum>secondNum){
	  int temp=secondNum;
	  secondNum=firstNum;
	  firstNum=temp;
	}
	mulsearch(first,firstNum,secondNum);

	  //printf("%d\n%s %s\n%d.%d.%d\n",node->val,node->name,node->surname,node->day,node->month,node->year);	
	   
      }
      break;
      }
    case ('d'):{
      int num; 
      scanf("%d",&num);
      first=delete(first,num);
      break;
    }
    case('r'):
      printf("reading\n");
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
