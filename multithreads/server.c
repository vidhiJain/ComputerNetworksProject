#include <stdio.h>
#include <string.h> // strlen
#include <stdlib.h> // strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> // write
#include <pthread.h> // for threading, link with lpthread
pthread_mutex_t myMutex;
pthread_cond_t cond;
pthread_mutex_t message;
// the thread function
void *connection_handler(void *);
struct user_arg    // struct to store arguments for thread
{
	int sock;
	char* in_message,out_message;
};
int main() {
	// Important declaration
    int serverSocket = 0, newSocket = 0;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddress;
    socklen_t addr_size;
    int randomNumber = 0, recvBytes = 0, recvNumber = 0,numThreads=0;
    
    /*  Create the network socket.   */
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    // Configure settings of the server address
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number */
    serverAddr.sin_port = htons(9000);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
    /* Set all bits of the padding field to 0 */
    memset ( serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero) );
    struct user_arg uarg[100];
    /*---- Bind the address struct to the socket ----*/
    if ( bind ( serverSocket, ( struct sockaddr *) &serverAddr, sizeof ( serverAddr ) )  < 0)
        printf("[ERROR] Socket binding Failed. \n");

    /* Listen on the socket, with 2 max connection requests queued */
    if ( listen ( serverSocket, 2 ) ==0 )
        printf ( "Server Scocket Initiated. Listening to its clients : \n" );
    else
        printf("[ERROR] More than limit.\n");
	
	pthread_t thread_id[100];
	while(numThreads<2&&(newSocket = accept ( serverSocket, (struct sockaddr *) &clientAddress, &addr_size))) {
		puts("Connection accepted");
		uarg[numThreads].sock=newSocket;
		uarg[numThreads].in_message=NULL;
		uarg[numThreads].out_message=NULL;
		if( pthread_create( &thread_id[numThreads], NULL,  connection_handler, (void*) &uarg[numThreads]) < 0) {
			perror("could not create thread");
			printf("HERE");
			return 1;
		}
		numThreads+=1;
		printf("%d",numThreads);
		// Now join the thread, so that we dont terminate before the thread
		//pthread_join( thread_id, NULL);
		puts("Handler assigned");
	}
	if (newSocket < 0) {
		perror("accept failed");
		printf("ERROR");
		return 1;
	}
	int y;
	// sleep(20);
	// for (y=0;y< 100;y++)
	// 	uarg[y].in_message="Hello";
	sleep(20);
	printf("Reached here");
	for (y=0;y< 100;y++)
		uarg[y].in_message="Hello2";
	for (y = 0; y < numThreads; y++)
       		pthread_join(thread_id[y], NULL);
	return 0;
}
 
//
// This will handle connection for each client
// 
void *connection_handler(void *x) {
	// Get the socket descriptor
	struct user_arg *dat = x;
	char recvBuffer[100],buffer[100];
	int recvNumber,randomNumber,recvBytes,pnum;
	int newSocket = dat->sock;
	int i;
	 if ( newSocket < 0)
            printf("[ERROR] Failed to accept the input.\n");
        send  ( newSocket, "k\0",2, 0 );
	while(1)
    {
       
        memset(recvBuffer, 0, 100);
        if(dat->in_message!=NULL)
        {
        	strcpy(buffer,dat->in_message);
        	send(newSocket,buffer,sizeof(buffer),0);
        	dat->in_message=NULL;
        }
        if ( ( recvBytes = recv ( newSocket, recvBuffer, 99, MSG_DONTWAIT) < 0)  );
           // printf("[ERROR] No input received from client.\n");
        else
        printf("Received Packet contains \"%s\"\n\n", recvBuffer);
        // printf("Sending packet %s\n",buffer);

        // send  ( newSocket, buffer, 10, 0 );
        // sleep ( 2);
        // dat->move_b=0;
        // dat->move_a=0;
        //printf("thread - %d ",newSocket);
        
    }
return;
} 
