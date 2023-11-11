#include "bst.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h> //can't believe I forgot this 
//#include <vcruntime.h>

struct stat fileStat;
struct dirent *currFile;

void processWord(char *word, Node **root);

void fileReader(char *path, Node **root) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int bufferSize = 256;
    char *processedWord = malloc(bufferSize);
    if (processedWord == NULL) {
        perror("Unable to allocate memory for processedWord");
        fclose(file);
        return;
    }

    int j = 0;
    int c;

    while ((c = fgetc(file)) != EOF) {
        // Resize processedWord if necessary
        if (j >= bufferSize - 1) {
            bufferSize *= 2;
            char *newBuffer = realloc(processedWord, bufferSize);
            if (newBuffer == NULL) {
                perror("Unable to allocate memory for processedWord");
                free(processedWord);
                fclose(file);
                return;
            }
            processedWord = newBuffer;
        }

        // Check for word termination
        if (isalpha(c) || c == '\'' || (c == '-' && j > 0 && isalpha(processedWord[j-1]))) {
            processedWord[j++] = c;
        } else if (j > 0) {
            processedWord[j] = '\0';
            processWord(processedWord, root);
            j = 0;
        }
    }

    // Process the last word if there is one
    if (j > 0) {
        processedWord[j] = '\0';
        processWord(processedWord, root);
    }

    free(processedWord);
    fclose(file);
}


void processWord(char *word, Node **root) {
    if (word[0] != '\0') { // Ensure the word is not empty
        *root = insertNode(*root, word);
    }
}

void recursiveFind(char *dPath, Node **root) {
    
    // Opens current directory
    DIR *dir = opendir(dPath);
    if (dir == NULL) {    
        perror("Error opening directory");
        return;
    } 

    struct dirent *currFile;
    while((currFile = readdir(dir)) != NULL) {
        // Checks if directory name is to deeper or higher and ignores this directory entry
        if (strcmp(currFile->d_name, ".") == 0 || strcmp(currFile->d_name, "..") == 0) {
            continue;
        }

        // Constructs full directory path
        char path[4096];// I wanted to use PATH_MAX to keep it more robust but its not working so we're just going to use 4096 
        snprintf(path, sizeof(path), "%s/%s", dPath, currFile->d_name);

        // Check if current file is a txt file
        if (stat(path, &fileStat) == 0) {
            if (S_ISDIR(fileStat.st_mode)) {
                printf("%s is a directory.\n", currFile->d_name);
                recursiveFind(path, root);
            } else if (strstr(currFile->d_name, ".txt") != NULL) {
                fileReader(path, root);
            }
        } else {
            perror("stat");
        }
    }
    
    if (closedir(dir) < 0) {
        perror("Error closing directory");
    }
}
void findTextFiles(int argc, char *argv[], Node **root) {
    --argc;  // Skip the program's own name
    while (argc > 0) {
        // Check if current file is a txt file or a directory
        if (stat(argv[argc], &fileStat) == 0) {
            if (S_ISDIR(fileStat.st_mode)) {
                printf("%s is a directory.\n", argv[argc]);
                recursiveFind(argv[argc], root);
            } else if (strstr(argv[argc], ".txt") != NULL) {
                fileReader(argv[argc], root);
            } else {
                printf("%s is not a .txt file.\n", argv[argc]);
            }
        } else {
            perror("stat");
        }
        --argc;
    }
}

int main(int argc, char *argv[]) {
      Node *root = NULL;

    // Checks whether user provided any files on runtime
    if (argc < 2) {
        printf("No files provided.\n");
    } else {
        // Process files and directories
        findTextFiles(argc, argv, &root);
    }

    // Count the total number of unique words
    int totalWords = countNodes(root);  
    WordCount *wordCounts = malloc(totalWords * sizeof(WordCount));

    if (wordCounts == NULL) {
        perror("Unable to allocate memory for word counts");
        return 1;
    }

    // Collect words and their counts from the BST
    int index = 0;
    inOrderCollect(root, wordCounts, &index);

    // Sort the words based on count and lexicographically
    qsort(wordCounts, totalWords, sizeof(WordCount), compareWordCounts);

    // Print the sorted words and their counts
    for (int i = 0; i < totalWords; i++) {
        printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
        free(wordCounts[i].word); // Free the duplicated word
    }

    // Free the array of WordCount and the BST
    free(wordCounts);
    freeBST(root);

    return 0;
}