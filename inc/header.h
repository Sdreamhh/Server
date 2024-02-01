#pragma once 

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
#include <sys/wait.h>


#include "color.h"
#include "log.h"
#include "server.h"
#include "decode.h"


#define HEADER_SIZE 2048
#define URI_SIZE 2048
