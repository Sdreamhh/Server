#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_SIZE 16

// ./cgi/adder?num1=11&num2=1
// runs so many times?
int main() {

    printf("\nCGI program adder started.\n");
    printf("QUERY_STRING: %s\n", getenv("QUERY_STRING"));

    char *query_string = getenv("QUERY_STRING");

    char str1[ARG_SIZE], str2[ARG_SIZE];
    
    char *p = strchr(query_string, '&');

    strncpy(str1, query_string, p - query_string);
    strncpy(str2, p + 1, strlen(query_string) - (p - query_string));

    str1[p - query_string] = '\0';

    printf("str1:%s str2:%s\n", str1, str2);

    char *argv1 = strchr(str1, '=') + 1;
    char *argv2 = strchr(str2, '=') + 1;

    printf("argv1: %s argv2: %s\n", argv1,argv2);

    int num1 = atoi(argv1);
    int num2 = atoi(argv2);

    printf("answer is: %d\n", num1 + num2);

    return 0;
}

