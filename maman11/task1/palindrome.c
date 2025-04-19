#include <stdio.h>
#include <string.h>

#define MAX_SIZE 80

/**
 * @brief This function checks if a string is a palindrome. A palindrome is a string that reads the same forwards and backwards.
 * @param s The string to be checked.
 * @return 1 if the string is a palindrome, 0 otherwise.
 * @note The function ignores spaces in the string.
 * @warning The function assumes that the input string is null-terminated and does not exceed MAX_SIZE.
 */
int palindrome(char s[]) {
    char str[MAX_SIZE]; // initialize string
    int i = 0; // initialize index for source string
    int length = 0; // initialize index for destination string

    // filter out spaces from the source string
    while (s[i] != '\0'){
        if(s[i] != ' '){ 
            str[length] = s[i];  // add character to string
            length++; // update destination string index
        }
        i++; // update source string index
    }
    str[length] = '\0'; // define end of string

    // compare characters from the start and end of the string
    for(i = 0; i < length / 2; i++){
        if(s[i] !=  s[length - i - 1]) {
            // if characters are not equal, return 0
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char str[MAX_SIZE]; // initialize string
    char c; // initialize character for input
    int i  = 0; // initialize index for string

    printf("Please enter string to be checked for palindromeness: "); // prompt user for input

    // read new character from standart input until EOF
    while((c=getchar()) != EOF){
        str[i] = c; // add character to string
        i++; // update index
    }
    
    str[i] = '\0'; // define end of string

    printf("\nYou entered: \"%s\"\n", str); // print input string

    int x = palindrome(str); // call to function to check if string is palindrome
    printf("Output: %d\n", x); // print result of palindrome check

    if (x == 1) {
        printf("The string is a palindrome\n");
    } else {
        printf("The string is not a palindrome\n");
    }

    return 0;
}
