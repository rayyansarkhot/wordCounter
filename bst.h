#ifndef BST_H
#define BST_H

typedef struct Node {
    char *word;
    int count;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(const char *word);
Node *insertNode(Node *root, const char *word);
void inOrderTraversal(Node *root);
void freeBST(Node *root);

#endif 
