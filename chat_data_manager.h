#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>

char* retrieve_userpass(char* username);

int update_socket(char* username,int sock);

int retrieve_socket(char* username);
