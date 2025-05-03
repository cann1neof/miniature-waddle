#include <stdio.h>

#define N 5

/* Struct that bundles all possible input‑parsing errors so the caller can
 * inspect them in one place instead of relying on several globals.           */
typedef struct errors {
    int error_NaN;
    int error_too_few;
    int error_too_many;
} errors;

/**
 * @brief This function reads a matrix of size N x N from standard input.
 * @param matrix The matrix to be filled with input values.
 * @return errors A struct containing error flags for NaN, too few, and too many inputs.
 * @note The function reads characters from standard input until EOF is reached or the matrix is filled.
 */
errors read_matrix(int matrix[N][N]){
    /* Initialize variables */
    int c;
    int counter = 0;
    int number = 0;
    int is_number = 0;
    int is_word_found = 0;

    /* Initialize the errors with zeros */
    errors err;
    err.error_NaN = 0;
    err.error_too_few = 0;
    err.error_too_many = 0;

    /* Read characters until EOF or the matrix is filled */
    while(counter < N*N){
        /* Read a character from standard input */
        c = getchar(); 

        /* Check for EOF */
        if(c == EOF){
            /* If EOF is reached and a number was found, store it in the matrix and end the cycle*/
            if(counter <= N*N - 1){
                matrix[counter / N][counter % N] = number;
            }
            break;
        }
        /* Check for newline, space, or tab */
        if(c == '\n' || c == ' ' || c == '\t'){
            /* If a number was found, store it in the matrix */
            if(is_word_found == 1 && is_number == 1){
                if(counter <= N*N - 1){
                    matrix[counter / N][counter % N] = number;
                }
                
                /* Reset the number and flags */
                is_number = 0;
                is_word_found = 0;
                number = 0;
                counter++;
            }
            /* If a number was not found, set the error flag */
            else if(is_word_found == 1 && is_number == 0){
                err.error_NaN = 1;
            }
            continue;
        }
        /* Check if the character is a digit */
        if(c >= '0' && c <= '9'){
            /* If a number was found, set the flag and update the number*/
            is_number = 1;
            is_word_found = 1;
            number = number * 10 + (c - '0');
        }
        else {
            /* If a non-digit character is found, set the error flag */
            is_number = 0;
            is_word_found = 1;
        }
    } 
    
    /* If amount of numbers is less than N*N update the error */
    if(counter < N*N - 1){
        err.error_too_few = 1;
    /* If amount of numbers is more than N*N update the error */
    } else if(counter > N*N - 1){
        err.error_too_many = 1;
    }
    /* Return errors */
    return err;
}

/**
 * @brief This function prints a matrix of size N x N to standard output.
 * @param matrix The matrix to be printerd.
 */

void print_matrix(int matrix[N][N]){
    /* Initialize variables */
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief This function checks if a matrix is a magic square.
 * @param matrix The matrix to be checked.
 * @return int The sum of the magic square if it is a magic square, 0 otherwise.
 * @note A magic square is a square matrix in which the sum of every row, column, and diagonal is the same.
 */

int is_magic_matrix(int matrix[N][N]){
    /* Initialize variables */
    int i, j;
    int sum = 0;
    int diag1 = 0;
    int diag2 = 0;
    int row_sum = 0;
    int col_sum = 0;

    /* check if the values are in the range 1 to N*N and unique */
    int check[N*N];
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(matrix[i][j] < 1 || matrix[i][j] > N*N || check[matrix[i][j] - 1] == 1){
                return 0;
            }
            check[matrix[i][j] - 1] = 1;
        }
    }

    /* Calculate the sum of the first row */
    for(i = 0; i < N; i++){
        sum += matrix[0][i];
    }

    /* Check rows */
    for(i = 0; i < N; i++){
        row_sum = 0;
        for(j = 0; j < N; j++){
            row_sum += matrix[i][j];
        }
        /* Check if the row sum is equal to the first row sum */
        if(row_sum != sum){
            return 0;
        }
    }


    /* Check columns */
    for(i = 0; i < N; i++){
        col_sum = 0;
        for(j = 0; j < N; j++){
            col_sum += matrix[j][i];
        }
        /* Check if the column sum is equal to the first row sum */
        if(col_sum != sum){
            return 0;
        }
    }
    /* Check diagonals */
    for(i = 0; i < N; i++){
        diag1 += matrix[i][i];
        diag2 += matrix[i][N - i - 1];
    }
    /* Check if the diagonal sums are equal to the first row sum */
    if(diag1 != sum || diag2 != sum){
        return 0;
    }
    /* If all checks passed, return the sum */
    return sum;
}


int main(int argc, char *argv[]){
    /* Initialize variables */
    int m[N][N];
    int flag = 0;

    /* Read the matrix */
    errors err = read_matrix(m);
   
    /* If any error is found, print the error message and return */
    if(err.error_NaN == 1 || err.error_too_few == 1 || err.error_too_many == 1){
        printf("Program terminated with one or multiple input errors. Please enter exactly %d numbers.\nList or errors:\n", N*N);
        if(err.error_NaN == 1){
            printf("\t[Error] NaN - You've entered symbol or symbols that is not a digit.\n");
        }
        if(err.error_too_few == 1){
            printf("\t[Error] Too few - You've entered to few numbers for a matrix.\n");
        }
        if(err.error_too_many == 1){
            printf("\t[Error] Too many - You've entered to many numbers for a matrix.\n");
        }
        return 0;
    }
    /* If no error is found, print the matrix and check if it is a magic square */
    else {
        printf("You've entered following matrix:\n");
        /* Print the matrix */
        print_matrix(m);
        /* Check if the matrix is a magic square */
        flag = is_magic_matrix(m);
        
        /* Print the result */
        if(flag == 0){
            printf("The matrix is not a magic square.\n");
        }
        else {
            printf("The matrix is a magic square, with sum = %d.\n", flag);
        }
    }

    return 0;
}

