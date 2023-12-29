#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>


#define MAXBUF 2048


#define END  "\033[0m"
#define CYAN "\033[1;36m"


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


/*
 Next step: 
        1. An interface 
        2. eval different pages 
        3. pthread programming
*/
int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "%susage: %s <port>%s\n", CYAN, argv[0], END);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);


    int serverfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* All local address */
    servaddr.sin_port = htons(port);

    bind(serverfd, (struct sockaddr *)(&servaddr), sizeof(servaddr));

    listen(serverfd, 128);
    printf("Server: waiting for connection on port %d...\n", port);


    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    while(1) {

        int clientfd = accept(serverfd, (struct sockaddr *)(&clientaddr), &clientlen);
        char addr[64];

        inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, addr, sizeof(addr));
        printf("Connected: client -> %s:%d\n", addr, ntohs(clientaddr.sin_port));


        char buf[1024] = "";
        recv(clientfd, buf, 1024, 0);
        printf("len: %ld\r\n", strlen(buf));
        printf("%s", buf);

        sendpage(clientfd, "index.html");
        
    }
    close(serverfd);


    return 0;
}