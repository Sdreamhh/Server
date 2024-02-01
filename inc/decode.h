#pragma once


static int map(char c);
void url_decode(char *src, char *dst);

void analyse_query_string(char *query_string, char *values[]);
void analyse_cmd(char *src, char *argv[]);


