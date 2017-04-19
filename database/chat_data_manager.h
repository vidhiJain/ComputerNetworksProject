#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>

char* retrieve_userpass(char* username);

int update_ipaddress(char* username,char* ipaddr);

char* retrieve_ipaddress(char* username);