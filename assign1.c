#include <stdio.h>
#include <time.h>

int main() {
    int n;
    clock_t start, end;
    double t;

    int inputs[] = {1000, 5000, 10000};

    for (int k = 0; k < 3; k++) {
        n = inputs[k];
        printf("\nInput size = %d\n", n);

        /* Constant Time O(1) */
        start = clock();
        int a = 5, b = 10, c;
        c = a + b;
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Constant time O(1): %f seconds\n", t);

        /* Linear Time O(n) */
        start = clock();
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum = sum + i;
        }
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Linear time O(n): %f seconds\n", t);

        /* Quadratic Time O(n^2) */
        start = clock();
        int count = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                count++;
            }
        }
        end = clock();
        t = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Quadratic time O(n^2): %f seconds\n", t);
    }

    return 0;
}