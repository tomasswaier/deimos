#include <stdio.h>
#include <stdlib.h>

void my_strcpy(char *dest, const char *src) {
    while (*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
}

struct Node {
    char name[20];
    char surname[20];
    int id;
    int day;
    int month;
    int year;
    struct Node *left;
    struct Node *right;
    int height;
};

int isFirst = 1;

struct Node *insertNode(struct Node *node, int id, char name[], char surname[], int day, int month, int year);
struct Node *deleteNode(struct Node *root, int id);
struct Node *search(struct Node *root, int id);
void multiSearch(struct Node *root, int id_start, int id_end);
int max(int a, int b);
int height(struct Node *N);
struct Node *newNode(int id, char name[], char surname[], int day, int month, int year);
struct Node *rightRotate(struct Node *y);
struct Node *leftRotate(struct Node *x);
int getBalance(struct Node *N);

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

struct Node *newNode(int id, char name[], char surname[], int day, int month, int year) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    my_strcpy(node->name, name);
    my_strcpy(node->surname, surname);
    node->id = id;
    node->day = day;
    node->month = month;
    node->year = year;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node *insertNode(struct Node *node, int id, char name[], char surname[], int day, int month, int year) {
    if (node == NULL)
        return (newNode(id, name, surname, day, month, year));

    if (id < node->id)
        node->left = insertNode(node->left, id, name, surname, day, month, year);
    else if (id > node->id)
        node->right = insertNode(node->right, id, name, surname, day, month, year);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && id < node->left->id)
        return rightRotate(node);

    if (balance < -1 && id > node->right->id)
        return leftRotate(node);

    if (balance > 1 && id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

struct Node *search(struct Node *root, int id) {
    if (root == NULL || root->id == id)
        return root;

    if (root->id < id)
        return search(root->right, id);
    else
        return search(root->left, id);
}


void multiSearch(struct Node *root, int id_start, int id_end) {
    if (root == NULL) {
        return;
    }

    if (root->id > id_start) {
        multiSearch(root->left, id_start, id_end);
    }

    if (root->id >= id_start && root->id <= id_end) {
        if (isFirst == 1) {
            printf("%d %s %s %d.%d.%d", root->id, root->name, root->surname, root->day, root->month, root->year);
            isFirst = 0;
        } else {
            printf("\n%d %s %s %d.%d.%d", root->id, root->name, root->surname, root->day, root->month, root->year);
        }
    }

    if (root->id < id_end) {
        multiSearch(root->right, id_start, id_end);
    }
}

struct Node *minValueNode(struct Node *node) {
    struct Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

struct Node *deleteNode(struct Node *root, int id) {
    if (root == NULL)
        return root;

    if (id < root->id)
        root->left = deleteNode(root->left, id);
    else if (id > root->id)
        root->right = deleteNode(root->right, id);
    else {
        if (root->left == NULL || root->right == NULL) {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            struct Node *temp = minValueNode(root->right);
            root->id = temp->id;
            my_strcpy(root->name, temp->name);
            my_strcpy(root->surname, temp->surname);
            root->day = temp->day;
            root->month = temp->month;
            root->year = temp->year;
            root->right = deleteNode(root->right, id);
        }
    }

    if (root == NULL)
        return root;

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

int main() {
    struct Node *root = NULL;
    char choice;

    while ((choice=getchar()) != EOF){
        switch (choice) {
            case 'i': {
                int id, day, month, year;
                char name[50], surname[50];
                scanf("%d %s %s %d.%d.%d", &id, name, surname, &day, &month, &year);
                root = insertNode(root, id, name, surname, day, month, year);
                break;
            }
            case 's': {
                int id_start, id_end;
                scanf("%d", &id_start);
                char x = getchar();
                if (x == '\n'|| x==EOF) {
                    struct Node *result = search(root, id_start);
                    if (result != NULL) {
                        if (isFirst == 1) {
                            printf("%d %s %s %d.%d.%d", result->id, result->name, result->surname, result->day, result->month, result->year);
                            isFirst = 0;
                        } else {
                            printf("\n%d %s %s %d.%d.%d", result->id, result->name, result->surname, result->day, result->month, result->year);
                        }
                    }
                } else {
                    scanf("%d", &id_end);
                    multiSearch(root, id_start, id_end);
                }
                break;
            }
            case 'd': {
                int id;
                scanf("%d", &id);
                root = deleteNode(root, id);
                break;
            }
        }
    }

    while (root != NULL) {
        root = deleteNode(root, root->id);
    }

    return 0;
}




