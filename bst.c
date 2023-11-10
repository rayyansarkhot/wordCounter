#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Node *createNode(const char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Unable to allocate memory for a new node");
        exit(EXIT_FAILURE);
    }

    newNode->word = strdup(word);
    if (newNode->word == NULL) {
        perror("Unable to allocate memory for the word");
        free(newNode);
        exit(EXIT_FAILURE);
    }

    newNode->count = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}
Node *insertNode(Node *root, const char *word) {
    // If the tree is empty, return a new node
    if (root == NULL) return createNode(word);

    // Otherwise, recur down the tree
    int cmp = strcmp(word, root->word);
    if (cmp < 0)
        root->left = insertNode(root->left, word);
    else if (cmp > 0)
        root->right = insertNode(root->right, word);
    else // If the word is already present in BST, increment the count
        root->count++;

    // Return the unchanged node pointer
    return root;
}
void inOrderTraversal(Node *root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%s: %d\n", root->word, root->count);
        inOrderTraversal(root->right);
    }
}
void freeBST(Node *root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root->word);
        free(root);
    }
}

