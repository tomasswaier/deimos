#include<stdio.h>
#include<stdlib.h>

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Person {
    int id;
    char first_name[20];
    char last_name[20];
    Date dob;
} Person;

struct Node {
    Person person;
    struct Node* left;
    struct Node* right;
    int ht;
};
typedef struct Node Node;

Node* root = NULL;

Node* createNode(Person);
Node* insert(Node*, Person);
Node* deleteNode(Node*, int);
Node* search(Node*, int);
void printNodesInRange(Node*, int, int, int *);
void freeTree(Node*);

int balanceFactor(Node*);
int height(Node*);
Node* rotateLeft(Node*);
Node* rotateRight(Node*);

// String copy function
void my_strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++))
        ;
}

int main() {
    char op;
    int id, day, month, year;
    char first_name[20], last_name[20];
    int i = 0;
    while (scanf(" %c", &op) != EOF) {
        if (op == 'i') {
            scanf("%d %s %s %d.%d.%d", &id, first_name, last_name, &day, &month, &year);
            Person person;
            person.id = id;
            my_strcpy(person.first_name, first_name);
            my_strcpy(person.last_name, last_name);
            person.dob.day = day;
            person.dob.month = month;
            person.dob.year = year;
            root = insert(root, person);
        } else if (op == 's') {
            int id1, id2;
            scanf("%d", &id1);
            if (scanf("%d", &id2) == 1) {
                if (id1>id2){
                    printNodesInRange(root, id2, id1, &i);
                }
                else{
                    printNodesInRange(root, id1, id2,& i);
                }
            }
            else {
                Node* node = search(root, id1);
                if (node != NULL){
                    if(i == 0) {
                        printf("%d %s %s %d.%d.%d", node->person.id, node->person.first_name, node->person.last_name, node->person.dob.day, node->person.dob.month, node->person.dob.year);
                        i++;
                    }else {
                        printf("\n%d %s %s %d.%d.%d", node->person.id, node->person.first_name, node->person.last_name, node->person.dob.day, node->person.dob.month, node->person.dob.year);
                    }
                }
            }
        } else if (op == 'd') {
            scanf("%d", &id);
            root = deleteNode(root, id);
        }
    }
    freeTree(root);
    return 0;
}

Node* createNode(Person person) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->person = person;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->ht = 1;
    return newNode;
}

int balanceFactor(Node* root) {
    if (root == NULL)
        return 0;
    return height(root->left) - height(root->right);
}

int height(Node* root) {
    if (root == NULL)
        return 0;
    return root->ht;
}

Node* rotateLeft(Node* root) {
    Node* rightChild = root->right;
    root->right = rightChild->left;
    rightChild->left = root;

    root->ht = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    rightChild->ht = 1 + ((height(rightChild->left) > height(rightChild->right)) ? height(rightChild->left) : height(rightChild->right));

    return rightChild;
}

Node* rotateRight(Node* root) {
    Node* leftChild = root->left;
    root->left = leftChild->right;
    leftChild->right = root;

    root->ht = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    leftChild->ht = 1 + ((height(leftChild->left) > height(leftChild->right)) ? height(leftChild->left) : height(leftChild->right));

    return leftChild;
}

Node* insert(Node* root, Person person) {
    if (root == NULL)
        return createNode(person);

    if (person.id < root->person.id)
        root->left = insert(root->left, person);
    else if (person.id > root->person.id)
        root->right = insert(root->right, person);
    else
        return root;

    root->ht = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    int balance = balanceFactor(root);

    if (balance > 1 && person.id < root->left->person.id)
        return rotateRight(root);
    if (balance < -1 && person.id > root->right->person.id)
        return rotateLeft(root);
    if (balance > 1 && person.id > root->left->person.id) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && person.id < root->right->person.id) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int id) {
    if (root == NULL)
        return root;

    if (id < root->person.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->person.id)
        root->right = deleteNode(root->right, id);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            root->person = temp->person;
            root->right = deleteNode(root->right, temp->person.id);
        }
    }

    if (root == NULL)
        return root;

    root->ht = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    int balance = balanceFactor(root);

    if (balance > 1 && balanceFactor(root->left) >= 0)
        return rotateRight(root);
    if (balance > 1 && balanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && balanceFactor(root->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && balanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* search(Node* root, int key) {
    if (root == NULL || root->person.id == key)
        return root;

    if (root->person.id < key)
        return search(root->right, key);

    return search(root->left, key);
}

void printNodesInRange(Node* root, int low, int high, int *i) {
    if (root == NULL)
        return;

    if (root->person.id > low)
        printNodesInRange(root->left, low, high,i);

    if (root->person.id >= low && root->person.id <= high) {
        if (i == 0) {
            printf("%d %s %s %d.%d.%d", root->person.id, root->person.first_name, root->person.last_name, root->person.dob.day, root->person.dob.month, root->person.dob.year);
            i++;
        } else {
            printf("\n%d %s %s %d.%d.%d", root->person.id, root->person.first_name, root->person.last_name, root->person.dob.day, root->person.dob.month, root->person.dob.year);
        }
    }

    if (root->person.id < high)
        printNodesInRange(root->right, low, high, i);
}

void freeTree(Node* root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
