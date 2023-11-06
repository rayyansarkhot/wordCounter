#include <stdio.h>
#include <sys/stat.h>

void findTextFiles(int argc, char *argv[]) {

    struct stat fileStat;

    // While there are files to be checked
    while(argc > 0){

        // Check if current file is a txt file
        if (stat(argv[1], &fileStat) == 0) {
            
            if (S_ISDIR(fileStat.st_mode)) {
                printf("%s is a directory.\n", argv[1]);
            } else {
                printf("%s is not a directory.\n", argv[1]);
            }

        } else {
            perror("stat");
            
        }

        break;
        
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

