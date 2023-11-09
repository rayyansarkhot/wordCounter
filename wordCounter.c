#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
// #include <vcruntime.h>

struct stat fileStat;
struct dirent *currFile;

void fileReader(char *path) {

    printf("%s is a file.\n", path);
    
}

void recursiveFind(char *dPath) {
    
    // Opens current directory
    DIR *dir;
    dir = opendir(dPath);
    
    if (dir == NULL) {    
        perror("Error opening directory");
    } else {
    
        // Reads a file from directory
        currFile = readdir(dir); 
        while(currFile != NULL) {

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
                    fileReader(path);
                }
                currFile = readdir(dir);

            } else {
                perror("stat");
            }

        }

    }
    
    closedir(dir);

}

void findTextFiles(int argc, char *argv[]) {

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

    // Checks whether user provided any files on runtime
    if (argc < 2) {
        printf("No files provided.\n");
    } else {

        // If there exists a range of files, it is sent to a method for 
        // individual txt file checking
        findTextFiles(argc, argv);
        
    }

    return 0;
}

