#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 1. allocation
int** allocate_matrix(int n) {
    int **matrix = calloc(n, sizeof(int*));
    if (matrix == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        matrix[i] = calloc(n, sizeof(int));
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

// 2.  freeing
void free_matrix(int **matrix, int n) {
    if (matrix == NULL) return;

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// 3. init
void input_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// 4. print
void print_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// 5. rotate
void rotate_90_clockwise(int **src, int n) {
   
    for(int i = 0; i < n - 1; i++) {
        for(int j = i + 1; j < n; j++){
            int tmp = src[i][j];
            src[i][j] = src[j][i];
            src[j][i] = tmp;
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n / 2; j++){
            int tmp = src[i][j];
            src[i][j] = src[i][n - j - 1];
            src[i][n - j - 1] = tmp;
        }
    }
    
}
int main() {
    int n;

    printf("Enter matrix size N (N x N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid matrix size\n");
        return 1;
    }

  
    int **src = allocate_matrix(n);

    if (src == NULL) {
        perror("Failed to allocate memory");
        free_matrix(src, n);
        return 1;
    }

    printf("Enter %dx%d matrix elements:\n", n, n);
    input_matrix(src, n);

   
    rotate_90_clockwise(src, n);

    printf("\nOriginal Matrix:\n");
    print_matrix(src, n);

    printf("\nRotated 90 Degrees Clockwise:\n");
    print_matrix(src, n);

   
    free_matrix(src, n);


    return 0;
}