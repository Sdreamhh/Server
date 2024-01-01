#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>



int main() {

    char filename[128], filetype[128], version[128];
    sscanf("GET / HTTP/1.1", "%s %s", filename, filetype);
    printf("%s %s|\n", filename, filetype);
    return 0;
}