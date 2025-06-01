/*
 * Matrix Calculator Program
 * A simple command-line calculator for 4x4 matrix operations
 */

#include <stdio.h>
#include "mymat.h"
#include "commands.h"

/* Main program - sets up matrices and starts the calculator */
int main(int argc, char *argv[]) {
    /* Define the 6 required individual matrix variables */
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;
    
    /* Create an array of these matrices to maintain compatibility with existing functions */
    mat matrices[MAT_COUNT];
    
    /* Initialize all individual matrices to zero */
    MAT_A = initialize_mat();
    MAT_B = initialize_mat();
    MAT_C = initialize_mat();
    MAT_D = initialize_mat();
    MAT_E = initialize_mat();
    MAT_F = initialize_mat();
    
    /* Populate the array with the individual matrices for function compatibility */
    matrices[0] = MAT_A;  /* MAT_A */
    matrices[1] = MAT_B;  /* MAT_B */
    matrices[2] = MAT_C;  /* MAT_C */
    matrices[3] = MAT_D;  /* MAT_D */
    matrices[4] = MAT_E;  /* MAT_E */
    matrices[5] = MAT_F;  /* MAT_F */

    /* Start processing user commands */
    process_commands(matrices);

    return 0;
}
