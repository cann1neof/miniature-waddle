#include <stdio.h>
#include <stdbool.h>

int getNumbers();
int creatingMagicSquare();
int printingMagicSquare();

int main() {
    int errorCode = getNumbers();
    if(errorCode == 0){
        printf("Your input is great! Starting building the magic square...\n");
    } else if(errorCode = 1){
        printf("You entered number that is not from the 1 to N^2\n");
    } else if(errorCode = 2){
        printf("Your input is great! Starting building the magic square...\n");
    }

    return 0;
}