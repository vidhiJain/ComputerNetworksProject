#include <unistd.h>
#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include <arpa/inet.h>
#include <time.h>

#define PORT 8324
#define BUF_SIZE 2000
#define USER_ID_SIZE 10
#define CLADDR_LEN 100
struct onlineUsers
{
  struct sockaddr_in cl;
  int userid;
  int sockfd;
  struct onlineUsers * next;
};
struct onlineUsersList
{
  struct onlineUsers* head;
  int size;
};
typedef struct onlineUsers onlineUsers;

typedef struct onlineUsersList onlineUsersList;
//maintaining a linked list
void insertOnlineUser(onlineUsersList* l, struct sockaddr_in cl, int userid, int sockfd){
  onlineUsers* obj = malloc(sizeof(onlineUsers));
  obj->cl = cl;
  obj->userid = userid;
  obj->sockfd = sockfd;
  obj->next = NULL;
  if(l->head == NULL){
    l->head = obj;
  }
  else{
    obj->next = l->head;
    l->head = obj;
    l->size++;
  }
}
onlineUsers * searchLinkedListOnFILE(onlineUsersList* l, int userid){
  onlineUsers * ptr = malloc(sizeof(ptr));
  FILE * fp;
  fp = fopen("userList.txt", "r");
  char time[30], ip[30];
  int id, port, socketfd;
  while(fp!=NULL){
    fscanf(fp, "%s,%d,%d,%s,%d",time, &id, &port, ip, &socketfd);
    if(userid == id){
      inet_aton(ip, &ptr->cl.sin_addr);
      // ptr->cl.sin_addr.s_addr = ;
      ptr->cl.sin_port = htons(port);
      ptr->cl.sin_family = AF_INET;
      ptr->sockfd = socketfd;
      ptr->userid = id;
    }
    return ptr;
  }
  // while(ptr!=NULL){
  //   if(ptr->userid == userid){
  //     return ptr;
  //   }
  //   ptr=ptr->next;
  // }
  // return NULL;
}
void printOnlineUserList(onlineUsersList*l, char * currtime){
  onlineUsers * ptr = l->head;
  FILE * fp;
  fp=fopen("userList.txt", "a");

  while(ptr!=NULL){
    char clientAddr[CLADDR_LEN];
    inet_ntop(AF_INET, &(ptr->cl.sin_addr), clientAddr, CLADDR_LEN);
    // inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
    printf("Timestamp: %s, Userid: %d, Port: %d, IP: %s, SocketFd: %d\n",currtime, ptr->userid, ntohs(ptr->cl.sin_port), clientAddr, ptr->sockfd );
    fprintf(fp,"%s,%d,%d,%s,%d\n",currtime, ptr->userid, ntohs(ptr->cl.sin_port), clientAddr, ptr->sockfd );
    ptr = ptr->next;
  }
  fclose(fp);
  return;
}
void error(int n){
  switch(n){
    case 1: {
              printf("Error creating socket!\n");
              exit(1);
            }
    case 2: {
              printf("Error binding!\n");
              exit(1);
            }
    case 3: {
              printf("Error accepting connection!\n");
              exit(1);
            }
    case 4: {
              printf("Error receiving data!\n");  
              exit(1);
            }
    case 5: {
              printf("Error receiving data!\n");  
              exit(1);
            }
    case 6: {
              printf("Error listening.\n");
              exit(1);
            }
    default: {
              printf("Error detected! Stopping the server\n");
              exit(1);
            }
  }
}
int main() {
  time_t currtime;

  onlineUsersList * l = malloc(sizeof(onlineUsersList));
  l->head = NULL;
  l->size = 0;
  struct sockaddr_in addr, cl_addr;
  int sockfd, len, ret, newsockfd;
  char buffer[BUF_SIZE];
  char userid[USER_ID_SIZE];

  pid_t childpid;
  char clientAddr[CLADDR_LEN];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) error(1);

  printf("Socket created...\n");

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = PORT;
  // printf("%s\n",INADDR_ANY );
  ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
  if (ret < 0) error(2);
  printf("Binding done...\n");
  printf("Listening on port %d...\n",PORT );
  
  ret = listen(sockfd, 5);
  if(ret < 0) error(6);
  printf("Waiting for a connection...\n");

  while(1) { //infinite loop
    // //clear the socket set 
    // FD_ZERO(&readfds);

    // //add master socket to set 
    // FD_SET(master_socket, &readfds);  
    // max_sd = master_socket;

    len = sizeof(cl_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
    if (newsockfd < 0) error(3);
    printf("Connection accepted...\n");
    

    inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
    if ((childpid = fork()) == 0) { //creating a child process
    

      close(sockfd); 
      //stop listening for new connections by the main process. 
      //the child will continue to listen. 
      //the main process now handles the connected client.
      memset(buffer, 0, BUF_SIZE);

      // //receiving secure file entry 
      // ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
      // if(ret < 0) error(4);
      // printf("Connection verified from %s: %s\n", clientAddr, buffer); 
      
      // //Asking user to enter the userid to whom to connect?
      // strcmp(buffer,"Enter the userId");
      // ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);   
      // if (ret < 0) error(5);

      char userid[USER_ID_SIZE];
      //Retrieving the user's self userid
      printf("Retrieving the userid of the user %s on port %d ... \n",clientAddr, ntohs(cl_addr.sin_port));
      memset(userid, 0, USER_ID_SIZE);
      ret = recvfrom(newsockfd, userid, USER_ID_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
      if(ret < 0) error(4);
      printf("Inserting user %s in online Users List...\n",userid );
      time(&currtime);
      insertOnlineUser(l, cl_addr, atoi(userid), newsockfd);
      printOnlineUserList(l, ctime(&currtime));

      //Receiving userid from the user to whom to connect to
      
      memset(userid, 0, USER_ID_SIZE);
      ret = recvfrom(newsockfd, userid, USER_ID_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
      if(ret < 0) error(4);
      printf("Client from %s on port %d has requested connection to userid %s\n", clientAddr,ntohs(cl_addr.sin_port), userid);
      

      printf("Checking if user %s is online or not for %s...\n", userid, clientAddr); 
      
      // function call returns integer a
      onlineUsers* client2 = searchLinkedListOnFILE(l, atoi(userid));
      // printOnlineUserList(l);

      if(client2 == NULL){
        printf("Saving the message : %s, as the user is not online.\n", buffer );
      }
      else{
       while(1){
          memset(buffer, 0, BUF_SIZE);
          ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
          if(ret < 0) error(4);
          ret = sendto(client2->sockfd, buffer, BUF_SIZE, 0 , (struct sockaddr *) &client2->cl, sizeof(client2->cl));
          if(ret < 0) error(5);
        }
      }
      // if(a==1){
      //   //exists.. connect
        
      // }
      // else{
      //   //does not exist ... save in the database
        
      // }

      //fetch ID from the db
      //
      printf(" to %s: %s\n", clientAddr, buffer);
      
      while(1){
        memset(buffer, 0, BUF_SIZE);
        ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
        if(ret < 0) error(4);
        //insert in db queue corresponding to the userid   
      }
    }
    
    close(newsockfd);
  }
}