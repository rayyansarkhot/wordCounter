#ifndef BST_H
#define BST_H

typedef struct Node {
    char *word;
    int count;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    char *word;
    int count;
} WordCount;


Node *createNode(const char *word);
Node *insertNode(Node *root, const char *word);
void inOrderTraversal(Node *root);
void freeBST(Node *root);
int compareWordCounts(const void *a, const void *b);
int countNodes(Node *root);
void inOrderCollect(Node *root, WordCount *wordCounts, int *index);
char *my_strdup(const char *s);
#endif 
