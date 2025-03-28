// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>

// Create Node
struct Node {
  int key;
  char name[20];
  char surname[20];
  int day;
  int month;
  int year;
  struct Node *left;
  struct Node *right;
  int height;
};
int isFirst = 1;
int max(int a, int b);

// Calculate height
int height(struct Node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max(int a, int b) { return (a > b) ? a : b; }
void strcpi(char *dest, const char *src) {
  while ((*dest++ = *src++) != '\0') {
    // Copy character by character until the null terminator is encountered
  }
}
// Create a node
struct Node *newNode(int key, char *name, char *surname, int day, int month,
                     int year) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->key = key;
  strcpi(node->name, name);
  strcpi(node->surname, surname);
  node->day = day;
  node->month = month;
  node->year = year;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;

  return (node);
}

// Right rotate
struct Node *rightRotate(struct Node *y) {
  struct Node *x = y->left;
  struct Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

// Left rotate
struct Node *leftRotate(struct Node *x) {
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}

// Get the balance factor
int getBalance(struct Node *N) {
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}

// Insert node
struct Node *insertNode(struct Node *node, int key, char *name, char *surname,
                        int day, int month, int year) {
  // Find the correct position to insertNode the node and insertNode it
  if (node == NULL)
    return (newNode(key, name, surname, day, month, year));

  if (key < node->key)
    node->left = insertNode(node->left, key, name, surname, day, month, year);
  else if (key > node->key)
    node->right = insertNode(node->right, key, name, surname, day, month, year);
  else
    return node;

  // Update the balance factor of each node and
  // Balance the tree
  node->height = 1 + max(height(node->left), height(node->right));

  int balance = getBalance(node);
  if (balance > 1 && key < node->left->key)
    return rightRotate(node);

  if (balance < -1 && key > node->right->key)
    return leftRotate(node);

  if (balance > 1 && key > node->left->key) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && key < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

struct Node *minValueNode(struct Node *node) {
  struct Node *current = node;

  while (current->left != NULL)
    current = current->left;

  return current;
}

// Delete a nodes
struct Node *deleteNode(struct Node *root, int key) {
  // Find the node and delete it
  if (root == NULL ||)
    return root;

  if (key < root->key)
    root->left = deleteNode(root->left, key);

  else if (key > root->key)
    root->right = deleteNode(root->right, key);

  else {
    if ((root->left == NULL) || (root->right == NULL)) {
      struct Node *temp = root->left ? root->left : root->right;

      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else
        *root = *temp;
      free(temp);
    } else {
      struct Node *temp = minValueNode(root->right);

      root->key = temp->key;
      strcpi(root->name, temp->name);
      strcpi(root->surname, temp->surname);
      root->day = temp->day;
      root->month = temp->month;
      root->year = temp->year;

      root->right = deleteNode(root->right, temp->key);
    }
  }

  if (root == NULL)
    return root;

  // Update the balance factor of each node and
  // balance the tree
  root->height = 1 + max(height(root->left), height(root->right));

  int balance = getBalance(root);
  if (balance > 1 && getBalance(root->left) >= 0)
    return rightRotate(root);

  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}
struct Node *search(struct Node *root, int num) {
  if (root == NULL) {
    return NULL;
  } else if (root->key == num) {
    return root;
  } else if (num > root->key)
    return search(root->right, num);
  else if (num < root->key)
    return search(root->left, num);
  return NULL;
}

void mulsearch(struct Node *node, int left, int right) {
  if (node == NULL)
    return;

  if (node->key < left) {
    mulsearch(node->right, left, right);
  } else if (node->key > right) {
    mulsearch(node->left, left, right);
  } else {
    mulsearch(node->left, left, right);
    if (node->key >= left && node->key <= right) {
      if (isFirst == 1)
        printf("%d %s %s %d.%d.%d", node->key, node->name, node->surname,
               node->day, node->month, node->year);
      else {
        printf("\n%d %s %s %d.%d.%d", node->key, node->name, node->surname,
               node->day, node->month, node->year);
        isFirst = 0;
      }
    }
    mulsearch(node->right, left, right);
  }
}

// Print the tree
void printPreOrder(struct Node *root) {
  if (root != NULL) {
    printf("%d ", root->key);
    printPreOrder(root->left);
    printPreOrder(root->right);
  }
}

int main() {
  struct Node *root = NULL;
  char uwu;

  while (((uwu = getchar()) != EOF)) {
    switch (uwu) {
    case ('i'): {
      int val, day, month, year;
      char name[20], surname[20];
      scanf("%d %s %s %d.%d.%d", &val, name, surname, &day, &month, &year);
      root = insertNode(root, val, name, surname, day, month, year);

      break;
    }
    case ('s'): {
      int num;
      scanf("%d", &num);
      char owo = getchar();
      if (owo == '\n' || owo == EOF) {
        struct Node *found = search(root, num);
        if (found != NULL) {
          if (isFirst == 1) {
            printf("%d %s %s %d.%d.%d", found->key, found->name, found->surname,
                   found->day, found->month, found->year);
            isFirst = 0;
          } else {
            printf("\n%d %s %s %d.%d.%d", found->key, found->name,
                   found->surname, found->day, found->month, found->year);
          }
        }
      } else {
        int num2;
        scanf("%d", &num2);
        if (num > num2) {
          int temp = num;
          num = num2;
          num2 = temp;
        }
        mulsearch(root, num, num2);
      }
      break;
    }
    case ('d'): {
      int num;
      scanf("%d", &num);
      root = deleteNode(root, num);
      break;
    }
    }
  }
  root = deleteNode(root, 3);

  return 0;
}
