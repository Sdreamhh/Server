#pragma once 

#include "header.h"


int start_server(int port);

void response(int fd, char *header);

static void request_dynamic(char *uri);

static void request_static(int fd, char * uri);
static void analyse_uri(char *uri, char *filename, char *filetype);
