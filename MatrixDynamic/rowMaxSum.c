#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// 1. memory alocation
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

// 2. memory freeing
void free_matrix(int **matrix, int n) {
    if (matrix == NULL) return;

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// 3. matrix initalisation
void input_matrix(int **matrix, int n, int m) {
    printf("Enter matrix elements (%dx%d):\n", n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// 4. maxsum row
void find_max_sum_row(int **matrix, int n, int m, int *maxSum, int *rowIndex) {
    *maxSum = INT_MIN;
    *rowIndex = 0;

    for (int i = 0; i < n; ++i) {
        int curMaxSum = 0;
        for (int j = 0; j < m; ++j) {
            curMaxSum += matrix[i][j];
        }
        if (i == 0 || curMaxSum > *maxSum) {
            *maxSum = curMaxSum;
            *rowIndex = i;
        }
    }
}

// 5. result print function
void print_result(int **matrix, int m, int rowIndex, int maxSum) {
    printf("Max Sum: %d\n", maxSum);
    printf("Row Index: %d\n", rowIndex);
    printf("Row Elements: ");
    for (int j = 0; j < m; ++j) {
        printf("%d ", matrix[rowIndex][j]);
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
    int maxSum, rowIndex;
    find_max_sum_row(matrix, n, m, &maxSum, &rowIndex);

    // Output
    print_result(matrix, m, rowIndex, maxSum);

    // Free
    free_matrix(matrix, n);

    return 0;
}