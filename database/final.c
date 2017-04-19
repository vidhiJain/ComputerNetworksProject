#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "chat_data_manager.h"

int main()
{
update_ipaddress("mohit","192.168.2.1");
printf("%s\n",retrieve_ipaddress("mohit"));
return(0);
}


