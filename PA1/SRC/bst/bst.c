#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node *newNode(int key) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->data = key;
    node->left = node->right = NULL;
    return node;
}

struct Node *insert(struct Node *root, int key) {
    if (root == NULL) {
        printf("inserted\n");
        return newNode(key);
    }

    if (key < root->data) {
        root->left = insert(root->left, key);
    } else if (key > root->data) {
        root->right = insert(root->right, key);
    } else {
        printf("not inserted\n");
    }

    return root;
}

struct Node *search(struct Node *root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }

    if (key < root->data) {
        return search(root->left, key);
    }

    return search(root->right, key);
}

void print(struct Node *root) {
    if (root != NULL) {
        printf("(");
        print(root->left);
        printf("%d", root->data);
        print(root->right);
        printf(")");
    }
}

struct Node *findMin(struct Node *node) {
    struct Node *current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct Node *deleteNode(struct Node *root, int key) {
    if (root == NULL) {
        printf("absent\n");
        return root;
    }

    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } else {
        printf("deleted\n");
        if (root->left == NULL) {
            struct Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node *temp = root->left;
            free(root);
            return temp;
        }

        struct Node *temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

void freeTree(struct Node *root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(int argc, char **argv) {
    struct Node *root = NULL;
    char cmd;
    int val;

    while (scanf("%c", &cmd) != EOF) {
        if (cmd == 'i') {
            scanf("%d", &val);
            root = insert(root, val);
        } else if (cmd == 's') {
            scanf("%d", &val);
            if (search(root, val) != NULL) {
                printf("present\n");
            } else {
                printf("absent\n");
            }
        } else if (cmd == 'p') {
            print(root);
            printf("\n");
        } else if (cmd == 'd') {
            scanf("%d", &val);
            root = deleteNode(root, val);
        }
    }
    freeTree(root);


    return 0;
}
