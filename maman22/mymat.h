#ifndef MYMAT_H
#define MYMAT_H

#include <stdio.h>
#include "command_queue.h"

#define MAT_COUNT 6  /* Number of matrices (A through F) */

typedef struct mat {
    double matrix[4][4];
} mat;

/* Matrix management functions */

/**
 * @brief Initializes a matrix with all elements set to zero
 * @return A matrix structure with all elements initialized to 0.0
 * @note This function creates a local matrix and returns it by value
 */
mat initialize_mat(void);

/**
 * @brief Converts matrix name to array index using ASCII arithmetic
 * @param name Matrix name in format "MAT_X" where X is A-F
 * @return Index (0-5) for valid matrix names, -1 for invalid names
 * @note Provides O(1) lookup time using ASCII arithmetic
 * @warning Returns -1 if name is NULL or doesn't follow the expected format
 */
int get_matrix_index(const char *name);

/**
 * @brief Gets a matrix pointer by name from the matrices array
 * @param name Matrix name in format "MAT_X" where X is A-F
 * @param matrices Array of matrices (MAT_A through MAT_F)
 * @return Pointer to the requested matrix, or NULL if name is invalid
 * @warning Returns NULL if name is invalid or index is out of bounds
 */
mat* get_matrix_by_name(const char *name, mat matrices[MAT_COUNT]);

/**
 * @brief Reads matrix values from command arguments and fills the target matrix
 * @param args Argument list containing matrix name and up to 16 numeric values
 * @param MAT Pointer to matrix to be filled with the parsed values
 * @note Expects matrix name as first argument, followed by numeric values
 * @note Values are filled sequentially row by row, ignoring extra arguments beyond 16
 * @warning Prints error messages for invalid arguments or missing matrix name
 */
void read_mat(arg_list *args, mat *MAT);

/**
 * @brief Prints matrix contents in formatted output
 * @param MAT Pointer to matrix to be printed
 * @note Output format: 4x4 grid with 8.2f formatting for each element
 * @warning Prints error message if MAT is NULL
 */
void print_mat(mat *MAT);

/**
 * @brief Performs matrix addition: dest_matrix = first_matrix + second_matrix
 * @param first_matrix First input matrix for addition
 * @param second_matrix Second input matrix for addition
 * @param dest_matrix Result matrix (can be same as input for in-place operation)
 * @note Safe for in-place operations since addition doesn't depend on previous results
 * @warning Prints error message if any matrix pointer is NULL
 */
void add_mat(mat *first_matrix, mat *second_matrix, mat *dest_matrix);

/**
 * @brief Performs matrix subtraction: dest_matrix = left_matrix - right_matrix
 * @param left_matrix Left operand matrix for subtraction
 * @param right_matrix Right operand matrix for subtraction (to be subtracted)
 * @param dest_matrix Result matrix (can be same as input for in-place operation)
 * @note Implemented using scalar multiplication and matrix addition
 * @warning Prints error message if any matrix pointer is NULL
 */
void sub_mat(mat *left_matrix, mat *right_matrix, mat *dest_matrix);

/**
 * @brief Performs matrix multiplication: dest_matrix = left_matrix * right_matrix
 * @param left_matrix Left operand matrix for multiplication
 * @param right_matrix Right operand matrix for multiplication
 * @param dest_matrix Result matrix (can be same as input for in-place operation)
 * @note Uses temporary matrix for in-place operations to avoid data corruption
 * @note Standard matrix multiplication algorithm with O(n^3) complexity
 * @warning Prints error message if any matrix pointer is NULL
 */
void mul_mat(mat *left_matrix, mat *right_matrix, mat *dest_matrix);

/**
 * @brief Performs scalar multiplication: dest_matrix = source_matrix * scalar
 * @param source_matrix Input matrix to be multiplied by scalar
 * @param scalar Scalar value to multiply each matrix element by
 * @param dest_matrix Result matrix (can be same as source for in-place operation)
 * @note Safe for in-place operations since each element is independent
 * @warning Prints error message if any matrix pointer is NULL
 */
void mul_scalar(mat *source_matrix, double scalar, mat *dest_matrix);

/**
 * @brief Performs matrix transposition: dest_matrix = transpose(source_matrix)
 * @param source_matrix Input matrix to be transposed
 * @param dest_matrix Result matrix (can be same as source for in-place operation)
 * @note For in-place transpose, swaps symmetric elements efficiently
 * @note For different matrices, performs direct transpose operation
 * @warning Prints error message if any matrix pointer is NULL
 */
void trans_mat(mat *source_matrix, mat *dest_matrix);

#endif /* MYMAT_H */

