#include <stdio.h>
#include <stdlib.h>

#define N 5

typedef struct errors {
    int error_NaN;
    int error_too_few;
    int error_too_many;
} errors;

errors read_matrix(int matrix[N][N]){
    char c;
    int counter = 0;
    int number = 0;
    int is_number = 0;
    int is_word_found = 0;

    errors err;
    err.error_NaN = 0;
    err.error_too_few = 0;
    err.error_too_many = 0;

    while(counter < N*N){
        c = getchar();

        if(c == EOF){
            if(counter < N*N){
                matrix[counter / N][counter % N] = number;
            }
            break;
        }
        if(c == '\n' || c == ' ' || c == '\t'){
            if(is_word_found == 1 && is_number == 1){
                if(counter < N*N){
                    matrix[counter / N][counter % N] = number;
                }
                
                is_number = 0;
                is_word_found = 0;
                number = 0;
                counter++;
            }
            else if(is_word_found == 1 && is_number == 0){
                err.error_NaN = 1;
            }
            continue;
        }

        if(c >= '0' && c <= '9'){
            is_number = 1;
            is_word_found = 1;
            number = number * 10 + (c - '0');
        }
        else {
            is_number = 0;
            is_word_found = 1;
        }
    } 
    
    if(counter < N*N - 1){
        err.error_too_few = 1;
    } else if(counter > N*N - 1){
        err.error_too_many = 1;
    }
    return err;
}

void print_matrix(int matrix[N][N]){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int is_magic_matrix(int matrix[N][N]){
    int i, j;
    int sum = 0;
    int diag1 = 0;
    int diag2 = 0;

    for(i = 0; i < N; i++){
        sum += matrix[0][i];
    }

    for(i = 0; i < N; i++){
        int row_sum = 0;
        for(j = 0; j < N; j++){
            row_sum += matrix[i][j];
        }
        if(row_sum != sum){
            return 0;
        }
    }

    for(i = 0; i < N; i++){
        int col_sum = 0;
        for(j = 0; j < N; j++){
            col_sum += matrix[j][i];
        }
        if(col_sum != sum){
            return 0;
        }
    }

    for(i = 0; i < N; i++){
        diag1 += matrix[i][i];
        diag2 += matrix[i][N - i - 1];
    }

    if(diag1 != sum || diag2 != sum){
        return 0;
    }

    return sum;
}


int main(void)
{
    int m[N][N];
    int flag = 0;
    errors err = read_matrix(m);
   
    if(err.error_NaN == 1 || err.error_too_few == 1 || err.error_too_many == 1){
        printf("Program terminated with one or multiple input errors. Please enter exactly %d numbers.\nList or errors:\n", N*N);
        if(err.error_NaN == 1){
            printf("\t[Error] NaN - You've entered symbol or symbols that is not a digit.\n");
        }
        if(err.error_too_few == 1){
            printf("\t[Error] Too few - You've entered to few symbols for a matrix.\n");
        }
        if(err.error_too_many == 1){
            printf("\t[Error] Too many - You've entered to many symbols for a matrix.\n");
        }
        return 1;
    }
    else {
        printf("You've entered following matrix:\n");
        print_matrix(m);
        flag = is_magic_matrix(m);
        if(flag == 0){
            printf("The matrix is not a magic square.\n");
        }
        else {
            printf("The matrix is a magic square, with sum = %d.\n", flag);
        }
    }

    return 0;
}

