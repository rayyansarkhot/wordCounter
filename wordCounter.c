#include <stdio.h>

int main(int argc, char *argv[]) {
    // Check if there are command-line arguments
    if (argc < 2) {
        printf("No command-line arguments provided.\n");
    } else {
        printf("Number of command-line arguments: %d\n", argc - 1);

        // Access and print each command-line argument
        for (int i = 1; i < argc; i++) {
            printf("Argument %d: %s\n", i, argv[i]);
        }
    }

    return 0;
}
