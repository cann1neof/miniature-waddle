#include "mymat.h"

mat initialize_mat() {
    mat MAT;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            MAT.matrix[i][j] = 0;
        }
    }
    return MAT;
}
