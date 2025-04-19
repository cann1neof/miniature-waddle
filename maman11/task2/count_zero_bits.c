#include <stdio.h>

/**
 * @brief This function counts the number of zero bits in an unsigned integer, including leading zeros.
 * @param num The unsigned integer to count zero bits in.
 * @return unsigned int The total number of zero bits in the number, including leading zeros.
 * @note The function counts the number of trailing zeros and leading zeros in the binary representation of the number.
 * @warning The function assumes that the input number is a non-negative integer.
 */
unsigned int count_zero_bits(unsigned int num){
    unsigned int counter = 0; // initialize counter
    unsigned int leading_zeros = 0; // initialize leading_zeros counter
    unsigned int num2 = num; // copy of num for counting leading zeros
    
    // count leading zeros
    while(num2 != 0){
        num2 /= 2;
        counter++;
    }
    leading_zeros = (sizeof(num2) * 8) - counter; // calculate number of leading zeros

    counter = 0; // reset counter for counting trailing zeros
    
    // count trailing zeros
    while(num != 0){
        if(num % 2 == 0){
            // if the last bit is 0, increment counter
            counter++;
        }
        // shift right by 1 bit
        num /= 2;
    }

    return counter + leading_zeros; // return total number of zeros
}

int main(int argc, char *argv[]) {
    unsigned int myNum;
    printf("Please enter your number: ");
    scanf("%d", &myNum);
    printf("\nYou enetered: %d\n", myNum);
    printf("Number of zero bits in this number including leading zeros is: %d\n", count_zero_bits(myNum));
    return 0;
}
