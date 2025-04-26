// Header file for input output functions
#include <stdio.h>
#define N 3


// Main function: entry point for execution
int main(int argc, char *argv[]) {
    
    char matrix[N][N];

    int i, j;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            // Reading matrix elements from user
            scanf("%c", &matrix[i][j]);
        }
    }

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            // Printing matrix elements
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    // Writing print statement to print hello world
    printf("Hello World");

    return 0;
}