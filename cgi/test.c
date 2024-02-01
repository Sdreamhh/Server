#include <stdio.h>

#include "decode.h"

int main() {

    char qs[] = "name1=hello&name2=sbming";

    char *values[10];

    printf("hello\n");
    analyse_query_string(qs, values);

    for(int i = 0; values[i]; i++) {
        printf("%s ", values[i]);
    }

    printf("end\n");

    return 0;
}