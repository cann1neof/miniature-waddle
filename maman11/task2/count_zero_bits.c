#include <stdio.h>

int count_zero_bits(int num){
    unsigned int counter;
    counter = 0;
    while(num != 0){
        if(num % 2 == 0){
            counter++;
        }
        num /= 2;
    }
    return counter;
}

int main() {
    unsigned int myNum;
    printf("enter your fucking number\n");
    scanf("%d", &myNum);
    printf("you have %d zero bits in your fucking number\n", count_zero_bits(myNum));
    return 0;
}
