/* lab4.c: Read integers and print out the sum of all even and odd
 * numbers separately */
#include <stdio.h>

int main() {
    int num = 0;
    int even_sum = 0;
    int odd_sum = 0;

     do {
        printf("Enter an integer: ");
        fflush(stdout);
        scanf("%d",&num);
        switch (num % 2) {
        case 0:
            even_sum += num;
            break;
        default:
            odd_sum += num;
        }
    } while (num != 0);

    printf("Sum of evens: %d\n", even_sum);
    printf("Sum of odds: %d\n", odd_sum);
    return 0;
}