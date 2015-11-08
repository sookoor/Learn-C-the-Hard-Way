/* lab5.c: Employee records */
#include <stdio.h>
#include <string.h>

struct Employee {
    char last[16];
    char first[11];
    char title[16];
    int salary;
};

int main() {
    struct Employee employees[BUFSIZ];
    for (int i = 0; i < BUFSIZ; i++) {
        printf("Enter the last name: ");
        fflush(stdout); /* To keep cursor on same line as prompt */
        gets(employees[i].last);
        if (strlen(employees[i].last) > 0) {
            printf("Enter the first name: ");
            fflush(stdout);
            gets(employees[i].first);
            printf("Enter the job title: ");
            fflush(stdout);
            gets(employees[i].title);
            printf("Enter the salary: ");
            fflush(stdout);
            scanf("%d", &employees[i].salary);
            getchar(); /* eat newline */
        } else {
            for (int j = 0; j < i; j++) {
                printf("%s %s, %s (%d)\n", employees[j].first, employees[j].last, employees[j].title, employees[j].salary);
            }
            break;
        }
    } 
}