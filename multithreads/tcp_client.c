/*
 *  Author: Sandeeep Kumar Mittal 
 *  Special Thanks to Prabodh Agarwal
 *  Description: Client side file to guess a number over TCP/IP sockets.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    // Important declaration
    int clientSocket;
    char *buffer;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // User Input
    char *userInput;

    userInput = (char *)malloc(sizeof(char)*10);
    buffer = (char *)malloc(sizeof(char)*100);

    /*  Create the network socket.   */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*  1) PR_INET Internet domain
        2) SOCK_STREAM represents Stream socket
        3) 0 represents Default protocol (TCP in this case)
        */

    // Configure settings of the server address
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;

    /* Set port number */
    serverAddr.sin_port = htons(9000);
    /*  In networking we prefer to use big-endian binary 
        sequencing.  Since in our PC we use small endian binary
        scheme, we use htons to convert little endian to big
        endian.  
        */

    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof
            serverAddr.sin_zero);

    /* Connect the socket to the server using the address*/
    addr_size = sizeof serverAddr;
    if ( connect( clientSocket, (struct sockaddr *) &serverAddr, 
                addr_size) < 0 )
    {
        printf("[ERROR] Unable to connect to server.\n");
        close( clientSocket );
        exit(0);
    }

    if ( recv ( clientSocket, buffer, 100, 0) < 0 )
    {
        printf("[ERROR] Connection closed prematurely.\n");
    }

    printf("Connection: %s\n",buffer);


    // recv ( clientSocket, buffer, 100, 0);
    // send(clientSocket, buffer, 100, 0);

      printf("Enter your message(s): ");

  while (fgets(buffer,100, stdin) != NULL) {
     if ( send(clientSocket, buffer, 100, 0) < 0 )
    {
        printf("[ERROR] Connection closed prematurely.\n");
    }
      if ( recv ( clientSocket, buffer, 100, 0) < 0 )
    {
        printf("[ERROR] Connection closed prematurely.\n");
    }
    printf("Received: ");
    fputs(buffer, stdout);
    printf("\n");
  }  
    // while(1)
    // {
    // if ( recv ( clientSocket, buffer, 100, 0) < 0 )
    // {
    //     printf("[ERROR] Connection closed prematurely.\n");
    //     break;
    // }
    // printf("%s\n",buffer );

    // }

    
    
    close( clientSocket );
    return 0;
}
