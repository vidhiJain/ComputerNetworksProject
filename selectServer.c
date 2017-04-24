/**
    Handle multiple socket connections with select and fd_set on Linux
*/
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "hashmap.h"
#include "chatbot.c"

#define TRUE   1
#define FALSE  0
#define PORT 8888

int main(int argc , char *argv[])
{
	Hashmap sockmap = hashmap_create(50);
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;
      
    char buffer[1025];  //data buffer of 1K
      
    //set of socket descriptors
    fd_set readfds;
      
    //a message
    char *message = "Yo \n";
  
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
      
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);
     
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
     
    while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        
            //send new connection greeting message
            // if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            // {
            //     perror("send");
            // }
              
            puts("Welcome message sent successfully");
              
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    hashmap_delete(sockmap,sd);
                   // hashmap_print(sockmap);
                    //delete from hashmap

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                  
                //Echo back the message that came in
                else
                {
                    
                    int outsock;
                    buffer[valread] = '\0';
                    char to_send[1025];
                    strcpy(to_send,buffer);
                    char* tok;
                    char tok1[20],tok2[20],tok3[1000];
                    tok= strtok(buffer,":");
                    strcpy(tok1,tok);
                    tok= strtok(NULL,":");
                    strcpy(tok2,tok);

                    tok= strtok(NULL,":");
                    strcpy(tok3,tok);
                    printf("HERE %s\n",tok3);
                    if(tok3!=NULL&&strcmp(tok1,"chatbot")==0&&strlen(tok3)>2)
                    {
                        FILE* fp=FindReccomendations(tok3);
                        char recc[1000];
                        while(GetReccomendations(recc,fp))
                        {
                            send(sd,recc, strlen(recc), 0 );
                        }


                    }
                    else
                    {
                    if(hashmap_search(sockmap,tok2)==-1)
                    {
                    	hashmap_insert(sockmap,tok2,sd);
                    	//hashmap_print(sockmap);
                    	// CODE HERE FOR READING FROM FILE
                        FILE* ftok2 = fopen(tok2, "r");
                        if(ftok2==NULL) {
                            //do nothing
                        }
                        else{
                            char history_chat[1025];
                            while(fgets(history_chat, sizeof(history_chat), ftok2)!=NULL){
                            	history_chat[strlen(history_chat)]='\0';
                            	sleep(1);
                                send(sd, history_chat, strlen(history_chat)+1, 0);
                                printf("Message in file ---| %s",history_chat);
                                memset(history_chat, 0, sizeof(history_chat));
                            }
                            fclose(ftok2);
                            int status = remove(tok2);
 
                            if( status == 0 )
                                printf("%s file deleted successfully.\n",tok2);
                            else
                            {
                                printf("Unable to delete the file\n");
                                perror("Error");
                            }
                        }
                        

                    }
                    if(hashmap_search(sockmap,tok1)==-1)
                    {
                    	printf("Receiver not online\n");
                    	// CODE HERE FOR WRITING TO FILE
                        FILE* ftok1 = fopen(tok1, "a");
                        int index=0;
                        while(to_send[index]!=':')
                            index++;
                        index++;
                        fputs(to_send+index, ftok1);
                        fputs("\n", ftok1);
                        fclose(ftok1);
                    }
                    else
                    {
                    outsock=hashmap_search(sockmap,tok1);
                    int index=0;
                    while(to_send[index]!=':')
                    	index++;
                    index++;
                    printf("sending on socket %d\n",outsock);
                    send(outsock , to_send + index, strlen(to_send) - index, 0 );
                	}
                	}
                }
            }
        }
    }
      
    return 0;
} 