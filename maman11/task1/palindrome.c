#include <stdio.h>
#include <string.h>

#define MAX_SIZE 120

int palindrome(char s[]) {
    int i;
    int length;
    length = strlen(s) - 1;
    for(i = 0; i <= length / 2; i++){
        if(s[i] !=  s[length - i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    char str[MAX_SIZE], c;
    int i;
    i = 0;
    while ( (c = getchar() ) != EOF ){
        if(c != ' ' && c != '\0'){
            str[i] = c;
            i++;
        }
    }
    str[i] = '\0';

    int x = palindrome(str);
    printf("output: %d\n", x);
    return 0;
}
