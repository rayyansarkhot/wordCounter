#include "bst.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>      // For open()
#include <unistd.h>     // For read() and close()

void processWord(char *word, Node **root);

void fileReader(char *path, Node **root) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    int bufferSize = 256;
    char *buffer = malloc(bufferSize);
    char *processedWord = malloc(bufferSize);
    if (buffer == NULL || processedWord == NULL) {
        perror("Unable to allocate memory");
        if (buffer) free(buffer);
        if (processedWord) free(processedWord);
        close(fd);
        return;
    }

    int bytesRead, j = 0;
    while ((bytesRead = read(fd, buffer, bufferSize - 1)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            char c = buffer[i];

            if (j >= bufferSize - 1) {
                bufferSize *= 2;
                char *newBuffer = realloc(processedWord, bufferSize);
                if (newBuffer == NULL) {
                    perror("Unable to allocate memory for processedWord");
                    free(buffer);
                    free(processedWord);
                    close(fd);
                    return;
                }
                processedWord = newBuffer;
            }

            if (isalpha(c) || c == '\'' || (c == '-' && j > 0 && isalpha(processedWord[j-1]))) {
                processedWord[j++] = c;
            } else if (j > 0) {
                processedWord[j] = '\0';
                processWord(processedWord, root);
                j = 0;
            }
        }
    }

    if (j > 0) {
        processedWord[j] = '\0';
        processWord(processedWord, root);
    }

    free(buffer);
    free(processedWord);
    close(fd);
}

void processWord(char *word, Node **root) {
    if (word[0] != '\0') {
        *root = insertNode(*root, word);
    }
}

void recursiveFind(char *dPath, Node **root) {
    DIR *dir = opendir(dPath);
    if (dir == NULL) {    
        perror("Error opening directory");
        return;
    }

    struct dirent *currFile;
    struct stat fileStat;

    while ((currFile = readdir(dir)) != NULL) {
        if (strcmp(currFile->d_name, ".") == 0 || strcmp(currFile->d_name, "..") == 0) {
            continue;
        }

        // Dynamically allocate memory for the path
        size_t dPathLen = strlen(dPath);
        size_t fileNameLen = strlen(currFile->d_name);
        size_t pathLen = dPathLen + fileNameLen + 2;  // +2 for the slash and null terminator
        char *path = malloc(pathLen);
        if (path == NULL) {
            perror("Unable to allocate memory for path");
            continue;
        }

        // Construct the path
        strcpy(path, dPath);
        path[dPathLen] = '/';
        strcpy(path + dPathLen + 1, currFile->d_name);

        // Process the path
        if (stat(path, &fileStat) == 0) {
            if (S_ISDIR(fileStat.st_mode)) {
                // Output and recursive call
                recursiveFind(path, root);
            } else if (strstr(currFile->d_name, ".txt") != NULL) {
                fileReader(path, root);
            }
        } else {
            perror("stat");
        }

        // Free the dynamically allocated memory
        free(path);
    }
    
    if (closedir(dir) < 0) {
        perror("Error closing directory");
    }
}
void findTextFiles(int argc, char *argv[], Node **root) {
    --argc;  // Skip the program's own name
    struct stat fileStat;

    while (argc > 0) {
        if (stat(argv[argc], &fileStat) == 0) {
            char *message;
            size_t messageLen;

            if (S_ISDIR(fileStat.st_mode)) {
                const char *dirMsg = " is a directory.\n";
                messageLen = strlen(argv[argc]) + strlen(dirMsg) + 1;
                message = (char *)malloc(messageLen);

                if (message) {
                    strcpy(message, argv[argc]);
                    strcat(message, dirMsg);
                    write(STDOUT_FILENO, message, strlen(message));
                    free(message);
                }
                
                recursiveFind(argv[argc], root);
            } else if (strstr(argv[argc], ".txt") != NULL) {
                fileReader(argv[argc], root);
            } else {
                const char *notTxtMsg = " is not a .txt file.\n";
                messageLen = strlen(argv[argc]) + strlen(notTxtMsg) + 1;
                message = (char *)malloc(messageLen);

                if (message) {
                    strcpy(message, argv[argc]);
                    strcat(message, notTxtMsg);
                    write(STDOUT_FILENO, message, strlen(message));
                    free(message);
                }
            }
        } else {
            const char errMsg[] = "Error in stat\n";
            write(STDERR_FILENO, errMsg, sizeof(errMsg) - 1);
        }
        --argc;
    }
}
void printNumber(int num, char *buffer, int *index) {
    if (num == 0) {
        buffer[(*index)++] = '0';
        return;
    }

    int start = *index;
    while (num != 0) {
        buffer[(*index)++] = '0' + (num % 10);
        num /= 10;
    }
    // Reverse the number in the buffer
    for (int i = start, j = *index - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }
}
int main(int argc, char *argv[]) {
    Node *root = NULL;

    // Checks whether user provided any files on runtime
    if (argc < 2) {
        const char noFilesMsg[] = "No files provided.\n";
        write(STDERR_FILENO, noFilesMsg, sizeof(noFilesMsg) - 1);
    } else {
        // Process files and directories
        findTextFiles(argc, argv, &root);
    }

    // Count the total number of unique words
    int totalWords = countNodes(root);  
    WordCount *wordCounts = malloc(totalWords * sizeof(WordCount));
    if (wordCounts == NULL) {
        const char allocErrMsg[] = "Unable to allocate memory for word counts\n";
        write(STDERR_FILENO, allocErrMsg, sizeof(allocErrMsg) - 1);
        return 1;
    }

    // Collect words and their counts from the BST
    int index = 0;
    inOrderCollect(root, wordCounts, &index);

    // Sort the words based on count and lexicographically
    qsort(wordCounts, totalWords, sizeof(WordCount), compareWordCounts);

    // Print the sorted words and their counts
    for (int i = 0; i < totalWords; i++) {
        int bufferSize = strlen(wordCounts[i].word) + 20; // Extra space for count and formatting
        char *outputBuffer = malloc(bufferSize);
        if (!outputBuffer) {
            continue; // Or handle memory allocation error
        }

        strcpy(outputBuffer, wordCounts[i].word);
        int bufferIndex = strlen(outputBuffer);
        outputBuffer[bufferIndex++] = ':';
        outputBuffer[bufferIndex++] = ' ';
        printNumber(wordCounts[i].count, outputBuffer, &bufferIndex);
        outputBuffer[bufferIndex++] = '\n';
        write(STDOUT_FILENO, outputBuffer, bufferIndex);

        free(outputBuffer);
        free(wordCounts[i].word); // Free the duplicated word
    }

    // Free the array of WordCount and the BST
    free(wordCounts);
    freeBST(root);

    return 0;
}