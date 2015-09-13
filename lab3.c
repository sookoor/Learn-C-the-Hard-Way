/* lab3.c: Read three integers and print out the sum of all even and odd
 * numbers separately */
#include <stdio.h>

int main() {
    int num = 0;
    int even_sum = 0;
    int odd_sum = 0;

    for (int i = 0; i < 3; i++) {
        printf("Enter an integer: ");
        fflush(stdout);
        scanf("%d",&num);
        if (num % 2 == 0) {
            even_sum += num;
        } else {
            odd_sum += num;
        }
    }

    printf("Sum of evens: %d\n", even_sum);
    printf("Sum of odds: %d\n", odd_sum);
    return 0;
}