#include "mymat.h"
#include "command_queue.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* Initialize a matrix with all zeros */
mat initialize_mat(void) {
    mat MAT;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            MAT.matrix[i][j] = 0;
        }
    }
    return MAT;
}

/* Convert matrix name like "MAT_A" to array index (0-5) */
int get_matrix_index(const char *name) {
    if (!name) return -1;
    
    /* Check if name follows "MAT_X" pattern where X is A-F */
    if (strlen(name) == 5 && 
        name[0] == 'M' && name[1] == 'A' && name[2] == 'T' && name[3] == '_') {
        
        char matrix_letter = name[4];  /* Extract the last character */
        
        /* Check if it's a valid matrix letter (A-F) */
        if (matrix_letter >= 'A' && matrix_letter <= 'F') {
            return matrix_letter - 'A';  /* ASCII arithmetic: A=0, B=1, C=2, etc. */
        }
    }
    
    return -1;  /* Invalid matrix name */
}

/* Get a pointer to the matrix by its name */
mat* get_matrix_by_name(const char *name, mat matrices[MAT_COUNT]) {
    int index = get_matrix_index(name);
    
    if (index >= 0 && index < MAT_COUNT) {
        return &matrices[index];
    }
    
    return NULL;  /* Invalid matrix name */
}

/* Check if a matrix contains invalid values (NaN or infinity) */
int is_matrix_valid(mat *matrix) {
    int i, j;
    
    if (!matrix) return 0;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (isnan(matrix->matrix[i][j]) || isinf(matrix->matrix[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

/* Read numbers from command arguments and fill the matrix */
void read_mat(arg_list *args, mat *target_matrix) {
    arg_node *current;
    int i, j, num_count;
    double value;
    char *arg_value, *endptr;
    char *arg_copy; /* For safe string manipulation */
    
    if (!args || !target_matrix) {
        printf("Error: Invalid arguments for read_mat\n");
        return;
    }
    
    current = get_first_argument(args);
    if (!current) {
        printf("Error: Matrix name missing for read_mat\n");
        return;
    }
    
    /* Skip matrix name, get numbers */
    current = get_next_argument(current);
    num_count = 0;
    
    /* Check if no numbers provided */
    if (!current) {
        printf("Note: No numbers provided - matrix remains unchanged\n");
        return;
    }
    
    /* Parse numbers and fill matrix sequentially */
    while (current && num_count < 16) {
        arg_value = get_argument_value(current);
        
        /* Create a copy for safe manipulation */
        if (arg_value && strlen(arg_value) > 0) {
            arg_copy = (char*)malloc(strlen(arg_value) + 1);
            if (!arg_copy) {
                printf("Error: Memory allocation failed during matrix reading\n");
                return;
            }
            strcpy(arg_copy, arg_value);
            
            /* Remove commas from copy */
            if (arg_copy[strlen(arg_copy)-1] == ',') {
                arg_copy[strlen(arg_copy)-1] = '\0';
            }
            
            value = strtod(arg_copy, &endptr);
            if (*endptr != '\0') {
                printf("Error: Invalid number '%s' in read_mat\n", arg_copy);
                free(arg_copy);
                return;
            }
            
            /* Check for overflow/underflow */
            if (value == HUGE_VAL || value == -HUGE_VAL) {
                printf("Error: Numeric overflow in value '%s'\n", arg_copy);
                free(arg_copy);
                return;
            }
            
            /* Check for NaN or infinity */
            if (isnan(value) || isinf(value)) {
                printf("Error: Invalid numeric value (NaN or infinity) in '%s'\n", arg_copy);
                free(arg_copy);
                return;
            }
            
            /* Fill matrix position by position (row by row) */
            i = num_count / 4;  /* Row index */
            j = num_count % 4;  /* Column index */
            target_matrix->matrix[i][j] = value;
            num_count++;
            
            free(arg_copy);
        }
        
        current = get_next_argument(current);
    }
    
    /* Provide feedback about matrix filling */
    if (num_count == 0) {
        printf("Note: No valid numbers provided - matrix remains unchanged\n");
    } else if (num_count < 16) {
        printf("Note: Only %d out of 16 values provided - remaining positions unchanged\n", num_count);
    } else if (current != NULL) {
        /* More than 16 arguments provided */
        printf("Note: Extra values beyond 16 were ignored\n");
    }
}

/* Print the matrix in a nice 4x4 format */
void print_mat(mat *MAT) {
    int i, j;
    
    if (!MAT) {
        printf("Error: Invalid matrix pointer for print_mat\n");
        return;
    }
    
    /* Check for invalid values before printing */
    if (!is_matrix_valid(MAT)) {
        printf("Error: Matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    
    printf("Matrix contents:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%8.2f ", MAT->matrix[i][j]);
        }
        printf("\n");
    }
}

/* Add two matrices together */
void add_mat(mat *first_matrix, mat *second_matrix, mat *target_matrix) {
    int i, j;
    
    if (!first_matrix || !second_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for add_mat\n");
        return;
    }
    
    /* Check for invalid values in source matrices */
    if (!is_matrix_valid(first_matrix)) {
        printf("Error: First matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    if (!is_matrix_valid(second_matrix)) {
        printf("Error: Second matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    
    /* Matrix addition: dest[i][j] = first[i][j] + second[i][j] */
    /* Safe to do in-place since addition doesn't depend on previous results */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            target_matrix->matrix[i][j] = first_matrix->matrix[i][j] + second_matrix->matrix[i][j];
            
            /* Check for overflow in result */
            if (isnan(target_matrix->matrix[i][j]) || isinf(target_matrix->matrix[i][j])) {
                printf("Error: Numeric overflow occurred during matrix addition\n");
                return;
            }
        }
    }
}

/* Subtract right matrix from left matrix */
void sub_mat(mat *left_matrix, mat *right_matrix, mat *target_matrix) {
    
    if (!left_matrix || !right_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for sub_mat\n");
        return;
    }

    /* Check for invalid values in source matrices */
    if (!is_matrix_valid(left_matrix)) {
        printf("Error: Left matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    if (!is_matrix_valid(right_matrix)) {
        printf("Error: Right matrix contains invalid values (NaN or infinity)\n");
        return;
    }

    /* Step 1: temp_matrix = right_matrix * (-1) */
    mul_scalar(right_matrix, -1, target_matrix);
    
    /* Step 2: target_matrix = left_matrix + temp_matrix */
    /*         which equals: target_matrix = left_matrix - right_matrix */
    add_mat(left_matrix, target_matrix, target_matrix);
}

/* Multiply two matrices using standard matrix multiplication */
void mul_mat(mat *left_matrix, mat *right_matrix, mat *target_matrix) {
    mat temp_matrix;
    int i, j, k;
    double sum;
    
    if (!left_matrix || !right_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for mul_mat\n");
        return;
    }
    
    /* Check for invalid values in source matrices */
    if (!is_matrix_valid(left_matrix)) {
        printf("Error: Left matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    if (!is_matrix_valid(right_matrix)) {
        printf("Error: Right matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    
    /* For matrix multiplication, we need to handle in-place operations carefully
     * since each result element depends on entire rows and columns from source matrices.
     * Use temporary matrix if destination overlaps with any source matrix. */
    if (target_matrix == left_matrix || target_matrix == right_matrix) {
        /* In-place operation: use temporary matrix to avoid data corruption */
        
        /* Perform multiplication into temporary matrix */
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                sum = 0;
                for (k = 0; k < 4; k++) {
                    sum += left_matrix->matrix[i][k] * right_matrix->matrix[k][j];
                }
                
                /* Check for overflow in intermediate result */
                if (isnan(sum) || isinf(sum)) {
                    printf("Error: Numeric overflow occurred during matrix multiplication\n");
                    return;
                }
                
                temp_matrix.matrix[i][j] = sum;
            }
        }
        
        /* Copy result back to destination matrix */
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                target_matrix->matrix[i][j] = temp_matrix.matrix[i][j];
            }
        }
    } else {
        /* Different matrices: direct multiplication */
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                sum = 0;
                for (k = 0; k < 4; k++) {
                    sum += left_matrix->matrix[i][k] * right_matrix->matrix[k][j];
                }
                
                /* Check for overflow in result */
                if (isnan(sum) || isinf(sum)) {
                    printf("Error: Numeric overflow occurred during matrix multiplication\n");
                    return;
                }
                
                target_matrix->matrix[i][j] = sum;
            }
        }
    }
}

/* Multiply every element in the matrix by a scalar value */
void mul_scalar(mat *source_matrix, double scalar, mat *target_matrix) {
    int i, j;
    double result;
    
    if (!source_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for mul_scalar\n");
        return;
    }
    
    /* Check for invalid values in source matrix */
    if (!is_matrix_valid(source_matrix)) {
        printf("Error: Source matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    
    /* Check for invalid scalar */
    if (isnan(scalar) || isinf(scalar)) {
        printf("Error: Invalid scalar value (NaN or infinity)\n");
        return;
    }
    
    /* Scalar multiplication: dest[i][j] = source[i][j] * scalar */
    /* Safe to do in-place since each element is independent */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result = source_matrix->matrix[i][j] * scalar;
            
            /* Check for overflow in result */
            if (isnan(result) || isinf(result)) {
                printf("Error: Numeric overflow occurred during scalar multiplication\n");
                return;
            }
            
            target_matrix->matrix[i][j] = result;
        }
    }
}

/* Transpose the matrix (flip it along the diagonal) */
void trans_mat(mat *source_matrix, mat *target_matrix) {
    int i, j;
    double temp;
    
    if (!source_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for trans_mat\n");
        return;
    }
    
    /* Check for invalid values in source matrix */
    if (!is_matrix_valid(source_matrix)) {
        printf("Error: Source matrix contains invalid values (NaN or infinity)\n");
        return;
    }
    
    if (source_matrix == target_matrix) {
        /* In-place transpose: swap symmetric elements */
        for (i = 0; i < 4; i++) {
            for (j = i + 1; j < 4; j++) {
                /* Swap matrix[i][j] with matrix[j][i] */
                temp = source_matrix->matrix[i][j];
                source_matrix->matrix[i][j] = source_matrix->matrix[j][i];
                source_matrix->matrix[j][i] = temp;
            }
        }
    } else {
        /* Different matrices: direct transpose */
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                target_matrix->matrix[j][i] = source_matrix->matrix[i][j];
            }
        }
    }
}
