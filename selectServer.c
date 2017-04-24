/**
    Handle multiple socket connections with select and fd_set on Linux
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "hashmap.h"
#include "chatbot.c"
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>

#define TRUE   1
#define FALSE  0
#define PORT 8888

int main(int argc , char *argv[])
{
	Hashmap sockmap = hashmap_create(50);
    int opt = TRUE;
    int sockm , addrl , client_lim = 23, sockn , sockc[30]  , actv, i , value_read , sd;
    int max_sockid;
    struct sockaddr_in sockaddrx;
      
    char buffer[1025];  //data buffer of 1K
      
    //set of socket descriptors
    fd_set readset_fd;
      
    //a hello_msg
    char *hello_msg = "Hello welcome to chat \n";
  
    //initialise all sockc[] to 0 so not checked
  	i=0;
    while(i < client_lim) 
    {

        sockc[i] = 0;
        i++;
    }
      
    //create a master socket
    if( (sockm = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        printf("failed in creating socket");
        exit(1);
    }
  
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(sockm, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("failed in setting master socket");
        exit(1);
    }
  
    //type of socket created
    sockaddrx.sin_family = AF_INET;
    sockaddrx.sin_addr.s_addr = INADDR_ANY;
    sockaddrx.sin_port = htons( PORT );
      
    //bind the socket to localhost port 8888
    if (bind(sockm, (struct sockaddr *)&sockaddrx, sizeof(sockaddrx))<0) 
    {
        perror("failed in binding socket");
        exit(1);
    }
    printf("Listening port %d \n", PORT);
     
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(sockm, 3) < 0)
    {
        printf("listening error");
        exit(1);
    }
      
    //accept the incoming connection
    addrl = sizeof(sockaddrx);
     int tempx;
    for(tempx=1;tempx>0;)
    {
        //clear the socket set
        FD_ZERO(&readset_fd);
  
        //add master socket to set
        FD_SET(sockm, &readset_fd);
        max_sockid = sockm;
         
        //add child sockets to set
        i=0;
        while( i < client_lim) 
        {
            //socket descriptor
            sd = sockc[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readset_fd);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sockid)
                max_sockid = sd;
            i++;
        }
  
        //wait for an actv on one of the sockets , timeout is NULL , so wait indefinitely
        actv = select( max_sockid + 1 , &readset_fd , NULL , NULL , NULL);
    
        if ((actv < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(sockm, &readset_fd)) 
        {
            if ((sockn = accept(sockm, (struct sockaddr *)&sockaddrx, (socklen_t*)&addrl))<0)
            {
                printf("connection accepted");
                exit(1);
            }
          
            //inform user of socket number - used in send and receive commands
                    
            //send new connection greeting hello_msg
            // if( send(sockn, hello_msg, strlen(hello_msg), 0) != strlen(hello_msg) ) 
            // {
            //     perror("send");
            // }
              
                       
            //add new socket to array of sockets
            i=0;
           	while( i < client_lim) 
            {
                //if position is empty
                if( sockc[i] == 0 )
                {
                    sockc[i] = sockn;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
                i++;
            }
        }
          
        //else its some IO operation on some other socket :)
         i=0;
           	while( i < client_lim) 
        {
            sd = sockc[i];
              
            if (FD_ISSET( sd , &readset_fd)) 
            {
                //Check if it was for closing , and also read the incoming hello_msg
                if ((value_read = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&sockaddrx , (socklen_t*)&addrl);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(sockaddrx.sin_addr) , ntohs(sockaddrx.sin_port));
                    hashmap_delete(sockmap,sd);
                   // hashmap_print(sockmap);
                    //delete from hashmap

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    sockc[i] = 0;
                }
                  
                //Echo back the hello_msg that came in
                else
                {
                    
                    int outsock;
                    buffer[value_read] = '\0';
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
            i++;
        }
    }
      
    return 0;
} 