#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "chat_data_manager.h"

int main()
{
update_message("mohit","Hello how are you?");
printf("%s\n",retrieve_message("mohit"));
return(0);
}


