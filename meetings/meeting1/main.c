// Header file for input output functions
#include <stdio.h>

// Main function: entry point for execution
int main() {
    int x;
    // Writing print statement to print hello world
    while (1){
        scanf("%d", &x);
        if(x % 7 == 0){
            printf("BOOM");
            continue;
        } else {
            while(x){
                if(x % 10 == 7){
                    printf("BOOM");
                    break;
                }
                x /= 10;
            }
        }
        
    }

    return 0;
}
