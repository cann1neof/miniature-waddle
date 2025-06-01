#include "mymat.h"
#include "command_queue.h"
#include <string.h>
#include <stdlib.h>

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

/* Read numbers from command arguments and fill the matrix */
void read_mat(arg_list *args, mat *target_matrix) {
    arg_node *current;
    int i, j, num_count;
    double value;
    char *arg_value, *endptr;
    
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
    
    /* Parse numbers and fill matrix sequentially */
    while (current && num_count < 16) {
        arg_value = get_argument_value(current);
        
        /* Remove commas and parse number */
        if (arg_value && strlen(arg_value) > 0) {
            if (arg_value[strlen(arg_value)-1] == ',') {
                arg_value[strlen(arg_value)-1] = '\0'; /* Remove trailing comma */
            }
            
            value = strtod(arg_value, &endptr);
            if (*endptr != '\0') {
                printf("Error: Invalid number '%s' in read_mat\n", arg_value);
                return;
            }
            
            /* Fill matrix position by position (row by row) */
            i = num_count / 4;  /* Row index */
            j = num_count % 4;  /* Column index */
            target_matrix->matrix[i][j] = value;
            num_count++;
        }
        
        current = get_next_argument(current);
    }
    
    /* If there are more than 16 arguments, ignore them (no error message needed) */
}

/* Print the matrix in a nice 4x4 format */
void print_mat(mat *MAT) {
    int i, j;
    
    if (!MAT) {
        printf("Error: Invalid matrix pointer for print_mat\n");
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
    
    /* Matrix addition: dest[i][j] = first[i][j] + second[i][j] */
    /* Safe to do in-place since addition doesn't depend on previous results */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            target_matrix->matrix[i][j] = first_matrix->matrix[i][j] + second_matrix->matrix[i][j];
        }
    }
}

/* Subtract right matrix from left matrix */
void sub_mat(mat *left_matrix, mat *right_matrix, mat *target_matrix) {
    
    if (!left_matrix || !right_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for sub_mat\n");
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
                target_matrix->matrix[i][j] = sum;
            }
        }
    }
}

/* Multiply every element in the matrix by a scalar value */
void mul_scalar(mat *source_matrix, double scalar, mat *target_matrix) {
    int i, j;
    
    if (!source_matrix || !target_matrix) {
        printf("Error: Invalid matrix pointers for mul_scalar\n");
        return;
    }
    
    /* Scalar multiplication: dest[i][j] = source[i][j] * scalar */
    /* Safe to do in-place since each element is independent */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            target_matrix->matrix[i][j] = source_matrix->matrix[i][j] * scalar;
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
