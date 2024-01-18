#include "server.h"



int start_server(int port) {

    /* socker -> bind -> lister : return serverfd*/

    int serverfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* All local address */
    servaddr.sin_port = htons(port);

    if(bind(serverfd, (struct sockaddr *)(&servaddr), sizeof(servaddr)) != 0) {
        fprintf(stderr, "%sBind error! Try again or try another port.\n%s", RED, END);
        exit(EXIT_FAILURE);
    }

    listen(serverfd, 128);
    printf("Server: waiting for connection on port %d...\n", port);

    return serverfd;
}


void response(int fd, char *header) {

    char method[10], uri[URI_SIZE], version[10];

    sscanf(header, "%s %s %s", method, uri, version);

    if(strstr(uri, "cgi")) {
        request_dynamic(fd, uri);
    }
    else {
        request_static(fd, uri);
    }
}


static void request_dynamic(int fd, char *uri) {

    printf("Debug: %s\n", uri);

    /* /cig/adder?num1=10&num2=20 -> prog: ./cgi/adder env: num1=10&num2=20 */
    char prog[64], env[128], *emptylist[] = { NULL };

    strcpy(prog, ".");

    char *p = strchr(uri, '?');

    strncpy(prog + 1, uri, p - uri);
    strncpy(env, p + 1, strlen(uri) - (p - uri));

    /* p - uri plus 1 relative to prog + 1 */
    prog[p - uri + 1] = '\0';

    printf("%s\n%s\n", prog, env);
   
    if(fork() == 0) {

        if(setenv("QUERY_STRING", env, 1) != 0) {
            fprintf(stderr, "%sSet env 'QUERY_STRING' failed!%s\n", RED, END);
        }
        dup2(fd, STDOUT_FILENO);
        execv(prog, emptylist);
    }
    wait(NULL);

}

static void request_static(int fd, char * uri) {

    char  filename[URI_SIZE], filetype[64];
    struct stat s_stat;

    analyse_uri(uri, filename, filetype);

    stat(filename, &s_stat);


    char *messgae[] = {
        "HTTP/1.1 200 OK\r\n",
        "Server: Milkv-duo server\r\n",
        "Connection: close\r\n",
        NULL
    };
    /* Send basic message */
    printf("Response header: \n");
    for(int i = 0; messgae[i]; i++) {
        send(fd, messgae[i], strlen(messgae[i]), 0);
        printf("%s", messgae[i]);
    }

    /* Send key message with format */
    char buf[MAXBUF];
    sprintf(buf, "Content-length: %ld\r\n", s_stat.st_size);
    send(fd, buf, strlen(buf), 0);
    printf("%s", buf);
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype);
    send(fd, buf, strlen(buf), 0);
    printf("%s", buf);


    /* Send files */
    int srcfd = open(filename, O_RDONLY, 0);
    printf("%s\n", filename);
    char *srcp = mmap(0, s_stat.st_size, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd);
    write(fd, srcp, s_stat.st_size);
    munmap(srcp, s_stat.st_size);
}



static void analyse_uri(char *uri, char *filename, char *filetype) {
   
    char *suffix[] = {".html",     ".jpg",      ".png",      ".ico",         ".css",     ".js",             ".webp",      ".gif",      NULL};
    char *MIME[]   = {"text/html", "image/jpg", "image/png", "image/x-icon", "text/css", "text/javascript", "image/webp", "image/gif", NULL};

    strcpy(filename, ".");

    /* Get filename */
    if(strcmp(uri, "/") == 0) 
        strcat(filename, "/index.html");    /* default */
    else 
        strcat(filename, uri);

    /* Get filetype(MIME) */
    for(int i = 0; ; i++) {
        if(suffix[i] == NULL) {
            printf("%sNo match MIME. Set default: text/plain. %s\n", RED, END);
            strcpy(filetype, "text/plain");
            break;
        }
        if(strstr(filename, suffix[i])){
            strcpy(filetype, MIME[i]);
            break;
        }
    }
}



