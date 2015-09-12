/* lab2.c: Rounds floating point number */
#include <stdio.h>

int main() {
    float fnum;

    printf("Enter a floating point number: ");
    fflush(stdout);
    scanf("%f",&fnum);

    printf("%f rounded is %d\n", fnum, (int)(fnum + 0.5));
    return 0;
}