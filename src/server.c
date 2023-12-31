#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>


#include "color.h"
#include "add.h"


#define MAXBUF 2048


void sendpage(int fd, char *filename);



void response(int fd, char *header);
void ana_uri(char *uri, char *filename, char *filetype);


int main(int argc, char *argv[]) {


    if(argc != 2) {
        fprintf(stderr, "%susage: %s <port>%s\n", CYAN, argv[0], END);
        exit(EXIT_FAILURE);
    }

    printf("Debug: %d\n", add(1, 2));

    int port = atoi(argv[1]);

    int serverfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* All local address */
    servaddr.sin_port = htons(port);

    //while(bind(serverfd, (struct sockaddr *)(&servaddr), sizeof(servaddr)) != 0);
    if(bind(serverfd, (struct sockaddr *)(&servaddr), sizeof(servaddr)) != 0) {
        fprintf(stderr, "%sBind error! Try again or try another port.\n%s", RED, END);
        exit(EXIT_FAILURE);
    }

    listen(serverfd, 128);
    printf("Server: waiting for connection on port %d...\n", port);


    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    while(1) {

        int clientfd = accept(serverfd, (struct sockaddr *)(&clientaddr), &clientlen);
        char addr[64];

        inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, addr, sizeof(addr));
        printf("%sConnected: client -> %s:%d%s\n", YELLOW, addr, ntohs(clientaddr.sin_port), END);

        char header[MAXBUF] = "";
        recv(clientfd, header, 1024, 0);
        printf("Header-length: %ld\n%s", strlen(header), header);

        response(clientfd, header);


        //sendpage(clientfd, "index.html");
        close(clientfd);
    }
    close(serverfd);

    return 0;
}



void ana_uri(char *uri, char *filename, char *filetype) {

    //printf("Debug: %s %s\n", uri, filename);
    
    strcpy(filename, ".");


    /* Get filename */
    if(strcmp(uri, "/") == 0) 
        strcat(filename, "/index.html");
    else 
        strcat(filename, uri);

    /* Get filetype(MIME) */
    if(strstr(filename, ".html"))  
        strcpy(filetype, "text/html");
    else if(strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if(strstr(filename, ".webp"))
        strcpy(filetype, "image/webp");
    else if(strstr(filename, ".ico"))
        strcpy(filetype, "image/x-icon");
    else 
        fprintf(stderr, "%sNo match MIME.%s\n", RED, END);
    
}


void response(int fd, char *header) {

    char method[MAXBUF], uri[MAXBUF], version[MAXBUF];
    char filetype[128], filename[MAXBUF];
    struct stat s_stat;

    //printf("Debug: %s\n", filename);

    sscanf(header, "%s %s %s", method, uri, version);

    /* Get filename and filetype */
    ana_uri(uri, filename, filetype);

    printf("Debug: %s %s\n", filename, filetype);

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






void sendpage(int fd, char *filename) {

    /* Get file information for st_szie */
    struct stat sbuf;
    stat(filename, &sbuf);

    char buf[MAXBUF];
    char *message[] = {
        "HTTP/1.1 200 OK\r\n",
        "Server: Milkv-duo server\r\n",
        "Connection: close\r\n",
        "Content-length: xxx\r\n",
        "Content-type: text/html\r\n\r\n"
    };

    /* Send response header */
    for(int i = 0; i < 5; i++) {
        if(i == 3) {
            sprintf(buf, "Content-length: %ld\r\n", sbuf.st_size);
            send(fd, buf, strlen(buf), 0);
            continue;
        }
        send(fd, message[i], strlen(message[i]), 0);
    }

    /* Send html page */

    FILE *pfile = fopen(filename, "r");

    char data[1024]; 
    do {
        fgets(data, 1024, pfile);
        send(fd, data, strlen(data), 0);
    } while(!feof(pfile));
    

}