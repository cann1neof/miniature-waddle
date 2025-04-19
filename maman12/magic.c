#include <stdio.h>
#include <stdbool.h>

#define N 3

typedef struct MyMessage {
    int errorMessage;
    int number;
} MyMessage;

MyMessage getNumbers(int *matrix[N][N]){
    int numbers_cash[N*N];
    MyMessage message;

    message.errorMessage = 0;
    message.number = 0;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            int current_index;
            current_index = i*N+j;

            scanf("%d", &numbers_cash[current_index]);
            if(numbers_cash[current_index] < 1 || numbers_cash[current_index] > N*N){
                message.errorMessage = 1;
                message.number = numbers_cash[current_index];
                return message;
            }

            for(int k = 0; k < current_index; k++){
                if(numbers_cash[k] == numbers_cash[current_index]){
                    message.errorMessage = 2;
                    message.number = numbers_cash[current_index];
                    return message;
                }
            }
            matrix[i][j] = &numbers_cash[current_index];
        }
    }
    return message;
}

bool checkMatrix(int matrix[N][N]){
    int sums[N*2+2];

    // Calculate the sums of the rows
    for(int i = 0; i < N; i++){
        sums[i] = 0;
        for(int j = 0; j < N; j++){
            sums[i] += matrix[i][j];
        }
    }

    // Calculate the sums of the columns
    for(int i = 0; i < N; i++){
        sums[N+i] = 0;
        for(int j = 0; j < N; j++){
            sums[N+i] += matrix[j][i];
        }
    }
    
    // Calculate the sum of the main diagonals
    sums[2*N] = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i == j){
                sums[2*N] += matrix[i][j];
            }
        }
    }

    // Calculate the sum of the secondary diagonals
    sums[2*N+1] = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i + j == N-1){
                sums[2*N+1] += matrix[i][j];
            }
        }
    }

    // Final check
    bool isMagic = true;
    for(int i = 1; i <= 2*N+1; i++){
        if(sums[i] != sums[i-1]){
            isMagic = false;
        }
    }

    return isMagic;
}

int main() {
    int matrix[N][N];
    MyMessage errorMessage = getNumbers(matrix);

    if(errorMessage.errorMessage == 1){
        printf("\nThe number %d is out of range\n", errorMessage.number);
        return 0;
    }

    if(errorMessage.errorMessage == 2){
        printf("\nThe number %d appears more than once\n", errorMessage.number);
        return 0;
    }

    printf("%s", checkMatrix(*matrix) ? "Magic square\n" : "Not a magic square\n");
    return 0;
}