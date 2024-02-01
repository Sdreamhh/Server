#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "decode.h"


int main() {

    

    char prog[32], *emptylist[] = { NULL };

    char *query_string = getenv("QUERY_STRING");

    printf("%s\n", query_string);


    char *p = strchr(query_string, '=');

    strncpy(prog, p + 1, strlen(query_string) - (p - query_string));


    printf("%s\n", prog);

    execvp(prog, emptylist);

    return 0;
}