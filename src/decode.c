#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "decode.h"

static int map(char c) {
    c = tolower(c);
    if(isdigit(c)) return c - '0';
    if(isalpha(c)) return c - 'a' + 10;
    return 0;
}

/* uri: ls%20-a, note: %20 is a hex */
void url_decode(char *src, char *dst) {

    char *ps = src, *pd = dst;

    while(*ps != '\0') {
        char tmp = *ps;

        if(tmp == '%') {
            char c1 = *(ps + 1);
            char c2 = *(ps + 2);
            tmp = map(c1) * 16 + map(c2);
            ps += 2;
        }
        *pd++ = tmp;
        ps++;
    }
    *pd = '\0';
}

// num1=10&num2=324&func=*
/* The source string will be written by strtok function */
void analyse_query_string(char *query_string, char *values[]) {

    char *delim = "&=";
    char *save_ptr[10];

    int i = 1;
    for(char *p = __strtok_r(query_string, delim, save_ptr); p != NULL; p = __strtok_r(NULL, delim, save_ptr), i++) {
        // if(i % 2 == 0) {
        //     strcpy(save_ptr[i], values[i / 2]);
        printf("%s ", save_ptr[i]);
    }
    values[i / 2 + 1] = NULL;

}



void analyse_cmd(char *src, char *argv[]) {

}




