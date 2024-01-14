#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include "log.h"
#include "color.h"

/* Log message too long */



static time_t s;
static char *t;
static struct stat s_stat;
static off_t off;

/* General code */
static int log_() {
    s = time((time_t *)NULL);
    t = ctime(&s);

    stat("log", &s_stat);

    int fd = open("log", O_RDWR, 0);

    if(fd == -1) {
        fprintf(stderr, "%sopen log file error!\n%s", RED, END);
    }

    off = lseek(fd, s_stat.st_size, SEEK_CUR);

    if(off == -1) {
        fprintf(stderr, "%sLog file off set error!\n%s", RED, END);
    }
    return fd;
}

/* Write start message to file log */
void log_start() {

    int fd = log_();

    char message[128];
    sprintf(message, "-----------\nThe server started at: %s-----------\n", t);
    write(fd, message, strlen(message));

    close(fd);
}

/* Write address message to file log */
void log_addr(char *addr) {

    printf("%sStamp: %s%s\n", GREEN, t, END);

    int fd = log_();

    write(fd, addr, strlen(addr));
    write(fd, "\t\t", 2);
    write(fd, t, strlen(t));
    write(fd, "\n", 1);

    close(fd);

}