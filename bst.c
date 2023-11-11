#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_strdup(const char *s) {
    char *copy = malloc(strlen(s) + 1);
    if (copy != NULL) {
        strcpy(copy, s);
    }
    return copy;
}
Node *createNode(const char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Unable to allocate memory for a new node");
        exit(EXIT_FAILURE);
    }

    newNode->word = my_strdup(word);
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

void inOrderCollect(Node *root, WordCount *wordCounts, int *index) {
    if (root != NULL) {
        inOrderCollect(root->left, wordCounts, index);
        wordCounts[*index].word = my_strdup(root->word); // Duplicate the word
        wordCounts[*index].count = root->count;
        (*index)++;
        inOrderCollect(root->right, wordCounts, index);
    }
}
int countNodes(Node *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int compareWordCounts(const void *a, const void *b) {
    const WordCount *wc1 = (const WordCount *)a;
    const WordCount *wc2 = (const WordCount *)b;

    // Primary sort by count in descending order
    if (wc1->count != wc2->count) {
        return wc2->count - wc1->count;
    }

    // Secondary sort lexicographically
    return strcmp(wc1->word, wc2->word);
}




void freeBST(Node *root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root->word);
        free(root);
    }
}

