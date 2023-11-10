#include "bst.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
//#include <vcruntime.h>

struct stat fileStat;
struct dirent *currFile;

void processWord(char *word, Node **root);

void fileReader(char *path, Node **root) {
    // Open the file
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char word[256];  // Buffer to hold each word, adjust size as needed
    while (fscanf(file, "%255s", word) == 1) {
        // Process each word
        char processedWord[256] = {0};
        int j = 0;
        
        // Removing leading non-letter characters
        int i = 0;
        while(word[i] && !isalpha(word[i])) i++;

        // Process the rest of the word
        for (; word[i]; i++) {
            if (isalpha(word[i]) || word[i] == '\'' || (word[i] == '-' && isalpha(word[i-1]) && isalpha(word[i+1]))) {
                processedWord[j++] = word[i];
            }
        }
        processedWord[j] = '\0'; // Null-terminate the processed word

        // Handle the processed word
        processWord(processedWord, root);
    }

    fclose(file);
}
void processWord(char *word, Node **root) {
    if (word[0] != '\0') { // Ensure the word is not empty
        *root = insertNode(*root, word);
    }
}

void recursiveFind(char *dPath) {
    
    // Opens current directory
    DIR *dir=NULL;
    dir = opendir(dPath);
    
    if (dir == NULL) {    
        perror("Error opening directory");
        return;
    } else {
    
        // Reads a file from directory
        //currFile = readdir(dir); 
        while((currFile = readdir(dir))!=NULL) {

            // Checks if directory name is to deeper or higher and ignores this directory entry
            if (strcmp(currFile->d_name, ".") == 0 || strcmp(currFile->d_name, "..") == 0) {
                currFile = readdir(dir);
                continue;
            }

            // Constructs full directory path
            char path[PATH_MAX]; 
            snprintf(path, sizeof(path), "%s/%s", dPath, currFile->d_name);

            // Check if current file is a txt file
            if (stat(path, &fileStat) == 0) {
                
                if (S_ISDIR(fileStat.st_mode)) {
                    
                    printf("%s is a directory.\n", currFile->d_name);
                    recursiveFind(path);

                } else {
                    // printf("%s is not a directory.\n", currFile->d_name);
                    //fileReader(path);
                     if (strstr(currFile->d_name, ".txt") != NULL) {
                        fileReader(path);
                    }
                }
                //currFile = readdir(dir);

            } else {
                perror("stat");
            }

        }

    }
    
    if (closedir(dir) < 0) {
        perror("Error closing directory");

}
}
void findTextFiles(int argc, char *argv[]) {
    --argc;
    // While there are files to be checked
    while(argc > 0){

        // Check if current file is a txt file
        if (stat(argv[argc], &fileStat) == 0) {
            
            if (S_ISDIR(fileStat.st_mode)) {
                printf("%s is a directory.\n", argv[argc]);
                char *dPath = argv[argc];
                recursiveFind(dPath);

            } else {
                printf("%s is a file.\n", argv[argc]);
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

        // If there exists a range of files, it is sent to a method for 
        // individual txt file checking
        findTextFiles(argc, argv);
        
    }
    inOrderTraversal(root);
    freeBST(root);
    return 0;
}

