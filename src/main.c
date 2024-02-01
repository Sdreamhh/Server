/* system headers and defines */
#include "header.h"

/* users' headers */
#include "color.h"
#include "log.h"
#include "server.h"


int main(int argc, char *argv[]) {


    if(argc != 2) {
        fprintf(stderr, "%susage: %s <port>%s\n", CYAN, argv[0], END);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);


    int serverfd = start_server(port);

    /* Write start message to file log */
    log_start();


    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    while(1) {

        int clientfd = accept(serverfd, (struct sockaddr *)(&clientaddr), &clientlen);
        char addr[64];

        inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, addr, sizeof(addr));
        printf("%sConnected: client -> %s:%d%s\n", YELLOW, addr, ntohs(clientaddr.sin_port), END);


        /* Write address message to file log */
        log_addr(addr);


        char header[HEADER_SIZE] = "";
        recv(clientfd, header, HEADER_SIZE, 0);
        printf("Header-length: %ld\n%s\nheader end\n", strlen(header), header);

        response(clientfd, header);

        close(clientfd);
    }
    close(serverfd);

    return 0;
}

