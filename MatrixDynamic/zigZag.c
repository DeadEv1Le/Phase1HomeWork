#include <stdio.h>
#include <stdlib.h>

// 1. memory allocation
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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// 4. zigzag
void print_zigzag(int **matrix, int n, int m) {
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(i % 2 == 0){
                printf("%d ", matrix[i][j]);
            }else{
                printf("%d ", matrix[i][m - j - 1]);
            }
        }
        printf ("\n");   
    }
} 
// void print_zigzag(int **matrix, int n, int m) {
//     for (int i = 0; i < n; i++) {
//         if (i % 2 == 0) {
//             // Ձախից աջ
//             for (int j = 0; j < m; j++) {
//                 printf("%d ", matrix[i][j]);
//             }
//         } else {
//             // Աջից ձախ (ցիկլը հետ ենք պտտում)
//             for (int j = m - 1; j >= 0; j--) {
//                 printf("%d ", matrix[i][j]);
//             }
//         }
//         printf("\n");   
//     }
// }

int main() {
    int n, m;

    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid n\n");
        return 1;
    }

    printf("Enter m: ");
    if (scanf("%d", &m) != 1 || m <= 0) {
        fprintf(stderr, "Invalid m\n");
        return 1;
    }

    int **matrix = allocate_matrix(n, m);
    if (matrix == NULL) {
        perror("Allocation failed");
        return 1;
    }

    printf("Enter matrix elements:\n");
    input_matrix(matrix, n, m);

    printf("\nZig-Zag Traversal:\n");
    print_zigzag(matrix, n, m);

    free_matrix(matrix, n);
    return 0;
}