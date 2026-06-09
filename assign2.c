#include <stdio.h>

int main() {
    int n = 5;

    int a = 10, b = 20, c;
    c = a + b;
    printf("Constant Space O(1)\n");

    int arr[5];
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    printf("Linear Space O(n)\n");

    int matrix[5][5];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = i + j;
        }
    }
    printf("Quadratic Space O(n^2)\n");

    return 0;
}