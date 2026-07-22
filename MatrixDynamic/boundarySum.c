#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// 1. memmory allocation
int** allocate_matrix(int n, int m) {
    int **matrix = calloc(n, sizeof(int*));
    if (matrix == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        matrix[i] = calloc(m, sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

// 2. memory free
void free_matrix(int **matrix, int n) {
    if (matrix == NULL) return;

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// 3. matrix init
void input_matrix(int **matrix, int n, int m) {
    printf("Enter matrix elements (%dx%d):\n", n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// 4. bound sum
void boundary_sum(int **matrix, int n, int m, int *sum) {
    if (matrix == NULL || sum == NULL) return;
    *sum = 0;
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if((i == 0 || i == n - 1 || j == 0 || j == m - 1) && i != j){
                *sum += matrix[i][j];
            }
        }
    }
}

int main() {
    int n, m;

    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid value for n\n");
        return 1;
    }

    printf("Enter m: ");
    if (scanf("%d", &m) != 1 || m <= 0) {
        fprintf(stderr, "Invalid value for m\n");
        return 1;
    }

    // Allocation
    int **matrix = allocate_matrix(n, m);
    if (matrix == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    // Input / Init
    input_matrix(matrix, n, m);

    // Algorithm
    int sum;
    boundary_sum(matrix, n, m, &sum);

    // Output
    printf("Boundary sum = %d\n", sum); 

    // Free
    free_matrix(matrix, n);

    return 0;
}