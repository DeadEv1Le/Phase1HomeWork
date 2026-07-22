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

// 4. col min sum
void find_min_sum_col(int **matrix, int n, int m, int *minSum, int *colIndex) {
    *minSum = INT_MAX;
    *colIndex = 0;

    for (int j = 0; j < m; ++j) {
        int curSum = 0;

        for (int i = 0; i < n; ++i) {
            curSum += matrix[i][j];
        }
        if (j == 0 || curSum < *minSum) {
            *minSum = curSum;
            *colIndex = j;
        }
    }
}

// 5. result print
void print_result(int **matrix, int n, int colIndex, int minSum) {
    printf("Column Index: %d\n", colIndex);
    printf("Column Sum: %d\n", minSum);
    printf("Column Elements: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", matrix[i][colIndex]);
    }
    printf("\n");
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
    int minSum, colIndex;
    find_min_sum_col(matrix, n, m, &minSum, &colIndex);

    // Output
    print_result(matrix, n, colIndex, minSum);

    // Free
    free_matrix(matrix, n);

    return 0;
}