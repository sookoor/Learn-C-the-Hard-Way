/* lab5.c: Employee records */
#include <stdio.h>
#include <string.h>

struct Employee {
    char last[16];
    char first[11];
    char title[16];
    int salary;
};

void stripNewline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n')
        str[len-1] = '\0';
}

int main() {
    struct Employee employees[BUFSIZ];
    for (int i = 0; i < BUFSIZ; i++) {
        printf("Enter the last name: ");
        fflush(stdout); /* To keep cursor on same line as prompt */
        fgets(employees[i].last, sizeof employees[i].last, stdin);
        stripNewline(employees[i].last);
        if (strlen(employees[i].last) > 0) {
            printf("Enter the first name: ");
            fflush(stdout);
            fgets(employees[i].first, sizeof employees[i].first, stdin);
            stripNewline(employees[i].first);
            printf("Enter the job title: ");
            fflush(stdout);
            fgets(employees[i].title, sizeof employees[i].title, stdin);
            stripNewline(employees[i].title);
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