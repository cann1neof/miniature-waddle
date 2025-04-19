#include <stdio.h>

unsigned int count_zero_bits(unsigned int num){
    unsigned int counter = 0;
    unsigned int length = 0;
    unsigned int num2 = num;
    // count leading zeros
    while(num2 != 0){
        num2 /= 2;
        counter++;
    }
    
    length = sizeof(num2) * 8 - length; // calculate number of leading zeros
    counter = 0;
    // count trailing zeros
    while(num != 0){
        if(num % 2 == 0){
            counter++;
        }
        num /= 2;
    }
    return counter + length; // return total number of zeros
}

int main() {
    unsigned int myNum;
    printf("Please enter your number: ");
    scanf("%d", &myNum);
    printf("\n You enetered: %d\n", myNum);
    printf("Number of zero bits in this number including leading zeros is: %d\n", count_zero_bits(myNum));
    return 0;
}
