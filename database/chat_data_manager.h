#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>

char* retrieve_userpass(char* username);

int update_message(char* username,char* ipaddr);

char* retrieve_message(char* username);
