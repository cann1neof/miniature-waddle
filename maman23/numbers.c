#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Converts 1-2 digit number string to English text representation
 * @param word Null-terminated string containing digits 0-99
 * @note Uses memory-optimized arrays with string reuse for teens
 * @warning Assumes valid digit input only
 */
void number_string_to_text(char *word) {
    /* C90 requires variable declarations at function start */
    int tens_digit, ones_digit, number;
    
    /* Base digits 0-9 for single numbers and compound construction */
    char *digits[] = {
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    
    /* Special cases that don't follow teen pattern */
    char *special_teens[] = {
        "ten", "eleven", "twelve"
    };
    
    /* Teen roots with NULL for reusable digits */
    char *teen_roots[] = {
        "thir",     /* 13: "thir" + "teen" = "thirteen" */
        NULL,       /* 14: digits[4] + "teen" = "fourteen" */
        "fif",      /* 15: "fif" + "teen" = "fifteen" (special spelling) */
        NULL,       /* 16: digits[6] + "teen" = "sixteen" */
        NULL,       /* 17: digits[7] + "teen" = "seventeen" */
        "eigh",     /* 18: "eigh" + "teen" = "eighteen" (special spelling) */
        NULL        /* 19: digits[9] + "teen" = "nineteen" */
    };
    
    /* Compressed tens array starting from twenty */
    char *tens_compressed[] = {
        "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
    };
    
    /* Handle single digit 0-9 */
    if (word[1] == '\0') {
        printf("%s\n", digits[word[0] - '0']);
        return;
    }
    
    /* Extract digits using ASCII arithmetic */
    tens_digit = word[0] - '0';
    ones_digit = word[1] - '0';
    number = tens_digit * 10 + ones_digit;
    
    /* Handle special teens 10-12 */
    if (number >= 10 && number <= 12) {
        printf("%s\n", special_teens[number - 10]);
        return;
    }
    
    /* Handle teens 13-19 with string reuse optimization */
    if (number >= 13 && number <= 19) {
        if (teen_roots[number - 13] != NULL) {
            printf("%steen\n", teen_roots[number - 13]);
        } else {
            printf("%steen\n", digits[ones_digit]);
        }
        return;
    }
    
    /* Handle 20-99 using compressed tens array */
    printf("%s", tens_compressed[tens_digit - 2]);
    if (ones_digit != 0) {
        printf(" %s", digits[ones_digit]);
    }
    printf("\n");
}

/**
 * @brief Main program entry point for number-to-text conversion
 * @param argc Number of command line arguments
 * @param argv Command line argument array
 * @note Supports stdin/stdout, input file, or input+output file modes
 * @warning Returns 1 on file operation errors or invalid argument count
 */
int main(int argc, char *argv[]) {
    /* Variables for character-by-character processing */
    int c;                    
    char word[3];            
    int word_pos = 0;        
    
    /* Handle command line arguments for file redirection */
    if(argc == 1) {
        /* Default: use stdin and stdout */
    }
    else if(argc == 2) {
        /* Redirect stdin from input file */
        if (freopen(argv[1], "r", stdin) == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
    }
    else if(argc == 3) {        
        /* Redirect stdin from input file and stdout to output file */
        if (freopen(argv[1], "r", stdin) == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        
        if (freopen(argv[2], "w", stdout) == NULL) {
            fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
            return 1;
        }
    }
    else {
        fprintf(stderr, "Error: Too many arguments provided. Expected 0-2 arguments.\n");
        return 1;
    }

    /* Process input character by character using dynamic programming */
    while ((c = getchar()) != EOF) {
        /* Whitespace delimits words */
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (word_pos > 0) {
                word[word_pos] = '\0';           
                number_string_to_text(word);
                word_pos = 0;
            }
        }
        else {
            /* Build word up to 2 characters */
            if (word_pos < 2) {
                word[word_pos] = c;
                word_pos++;
            }
            
            /* Output word when it reaches maximum length */
            if (word_pos == 2) {
                word[word_pos] = '\0';           
                number_string_to_text(word);
                word_pos = 0;
            }
        }
    }
    
    /* Handle final word if input doesn't end with whitespace */
    if (word_pos > 0) {
        word[word_pos] = '\0';
        number_string_to_text(word);
    }
    
    return 0;
}
