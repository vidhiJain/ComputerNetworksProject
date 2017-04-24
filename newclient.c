#include <pthread.h>
#include <curses.h>
#include <termios.h>
#include <string.h>
#include "ui.c"
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>



int finished_chat=0; 

//char name[20];
char name[20];
int serverDownflag =0;
int sockfd;
WINDOW *top;
WINDOW *bottom;
int line=1; // Line position of top
int input=1; // Line position of top
int maxx,maxy; // Screen dimensions
pthread_mutex_t func_mut_th = PTHREAD_MUTEX_INITIALIZER;  
FILE * file;

void *msg_send_func();

void *msg_listen_func();
int loginflag=0;

char toSend[20];
int main()
{
    while(1){

     //printf("Enter user name: ");
    
 //scanf("%s",name);
        system("clear");
        int lcnt=0;
        if(loginflag!=1){
    while(!login(name) )
        {
            lcnt++;
            if(lcnt==3)
            {
                printf("Contact Your Admin for Correct Credentials\n");
                exit(0);
            }
        };
    }
    loginflag=1;
    int caseflag=1;
    while(caseflag)
    {
    system("clear");
    reset();
    //char name[30]="Test";

    //getname(name);
    if(serverDownflag == 1){
        printf("Server Down! Restart your application.\n");
        serverDownflag=0;
        exit(0);
    }
    printf("Welcome %s\n",name);
    printf("Choose One of the following options:\n");
    printf("1.Start a new chat \t");
    printf("2.View an old chat \t");
    printf("3.Delete an old chat\n");
    int o=0;
    scanf("%d",&o);
    printf("%d\n",o );
    switch(o)
    {
        case 1:system("clear");
                printf("Please Enter Name of the person you want to chat with \n");
                 FILE * file1;
                 file1 = fopen( "logdata/contacts" , "r");
                 
                if (file1) {
                                    char ch;
                                    system("clear");
                                    while( ( ch = fgetc(file1) ) != EOF ){
                                         printf("%c",ch);
                                        }
                                 fclose(file1);
                            }
                            else printf("File Error in 1\n");
                // char temp12[30];
                 scanf("%s",toSend);
                 char temp2[30]="data/";     
                strcat(temp2,toSend);
                file1 = fopen( temp2 , "w+"); // take user id from server and write to file

                caseflag=0;

                break;
        case 2: system("clear");
                printf("Please Name the chat you want to open \n");
                listdir();
                printf("\n");
                int notopen=1;
                char temp[30];

                while(notopen){
                                scanf("%s",temp);
                                //char str[999];
                                 
                                 char temp1[30]="data/";    
                                 strcat(temp1,temp);
                                 file = fopen( temp1 , "r");
                                 if (file) {
                                    notopen=0;
                                    char ch;
                                    system("clear");
                                 // int color=0;
                                    // int id_index=0;
                                    
                                    // for(;id_index<15;id_index++){
                                    //     ch = fgetc(file);
                                    //     desid[id_index]=ch;
                                    // }
                                    // ch = fgetc(file);
                                    // desid[17]='\0';
                                    //printf("%s\n",desid );
                                    while( ( ch = fgetc(file) ) != EOF ){

                                        if(ch=='|') 
                                        {
                                            ch = fgetc(file);
                                            if(ch=='^')
                                            green();
                                            else if (ch=='*') blue();
                                            ch = fgetc(file);
                                        }
                                         printf("%c",ch);
                                        }
                                 fclose(file);
                                 reset();
                                 printf("Enter 1 to continue\n");
                                 scanf("%s",temp);
                                //writetochatlog(temp,"You","Kuchtohtime",0,temp1);
                                 //return desid;
                                    }
                                    else
                                        printf("Please enter correct chatname \n");
                                    }
                                    caseflag=1;

                break;
        case 3: system("clear");
                printf("Please Name the chat you want to delete \n");
                listdir();
                printf("\n");
                scanf("%s",temp);
                FILE * file;
                char temp1[30]="data/";     
                strcat(temp1,temp);
                if(!remove(temp1)) 
                {
                    char c;
                    printf("Chat deleted.Enter to continue \n");
                    //usleep(1000);
                    scanf("%c",&c);
                    scanf("%c",&c);
                    //usleep(100);
                    //home(); // recursive calls!! dangeorus
                }
                else
                    printf("Please enter correct chatname \n");
                caseflag=1;
                break;
    }
}
   // printf("\nEnter user to send to: ");
    
   //scanf("%s",toSend);

    // save old terminal settings
    struct termios terminal_old, terminal_new;
    tcgetattr ( 0, &terminal_old );
    terminal_new = terminal_old;

    // Disable echo and cannon
    terminal_new.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( 0, TCSANOW, &terminal_new );


    char password[10];
    char *in = password;

    char c;
    int i=0;
  
    // Reset terminal
    tcsetattr ( 0, TCSANOW, &terminal_old );  
    // start building connection
    int len;
    int conresult;
    char buf[256];
    struct sockaddr_in addr_sock;

    // Make socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // attr
    addr_sock.sin_family = AF_INET;
    addr_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_sock.sin_port = htons(8888);

    len = sizeof(addr_sock);

    // Make connection
    conresult = connect(sockfd, (struct sockaddr *)&addr_sock, len);

    printf("\n\nBuilding connection\n\n");
    switch(conresult)
    {
    	case -1:
    	printf("\nFailed Connection , please try again.\n");
        exit(1);
        break;
        default:
        printf("\nconnected successfully\n");

    }

    initscr();  
    getmaxyx(stdscr,maxy,maxx);

    top = newwin(maxy/2,maxx,0,0);
    bottom= newwin(maxy/2,maxx,maxy/2,0);

    scrollok(top,TRUE);
    scrollok(bottom,TRUE);
    box(top,'|','=');
    box(bottom,'|','-');

    wsetscrreg(top,1,maxy/2-2);
    wsetscrreg(bottom,1,maxy/2-2);


    // Set up threads
    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Spawn the listen/receive deamons
    pthread_create(&threads[0], &attr, msg_send_func, NULL);
    pthread_create(&threads[1], &attr, msg_listen_func, NULL);

    // Keep alive until finish condition is finished_chat
    while(!finished_chat);
    tcsetattr ( 0, TCSANOW, &terminal_old );
    //while(1);
}
}

// Send message from keyboard to server and update screen
void *msg_send_func()
{

    char str[80];
    char msg[100];
    int bufsize=maxx-4;
    char *buffer=malloc(bufsize);
    bzero(str,80);
    bzero(msg,100);
    strcpy(msg,toSend);
    strncat(msg,":\0",100-strlen(" "));
    strncat(msg,name,100-strlen(" "));
    strncat(msg,":\0",100-strlen(" "));
    strncat(msg," ",100-strlen(" "));
    write(sockfd,msg,strlen(msg));
    while(1)
    {
        bzero(str,80);
        bzero(msg,100);
        bzero(buffer,bufsize);
        wrefresh(top);
        wrefresh(bottom);

        // Get user's message
        mvwgetstr(bottom,input,2,str);

        // Build the message: "name: message"
        strcpy(msg,toSend);
        strncat(msg,":\0",100-strlen(str));
        strncat(msg,name,100-strlen(str));
        strncat(msg,":\0",100-strlen(str));
        strncat(msg,str,100-strlen(str));
        //writetochatlog(msg,0,toSend);
        write(sockfd,msg,strlen(msg));
        // Check for quiting
        if(strcmp(str,"quit()")==0)
        {

            finished_chat = 1;

            // Clean up
            endwin();      
            pthread_mutex_destroy(&func_mut_th);
            pthread_exit(NULL);
            close(sockfd);
        }    

   
      /* close the file */
      //fclose(fp);
        

        bzero(msg,100);
        strncat(msg,name,100-strlen(str));
        strncat(msg,":\0",100-strlen(str));
        strncat(msg,str,100-strlen(str));
        // write it in chat window (top)
        mvwprintw(top,line,2,msg);
        writetochatlog(msg,0,toSend);
        
       // sleep(5);


        // scroll the top if the line number exceed height
        pthread_mutex_lock (&func_mut_th);

        if(line==maxy/2-2)
            scroll(top);

        else
            line++;

        // scroll the bottom if the line number exceed height
        if(input==maxy/2-2)
            scroll(bottom);
        else
        	input++;
            

        pthread_mutex_unlock (&func_mut_th);
    }
}


// Listen for messages and display them
void *msg_listen_func()
{
    
    char str[80];
    int bufsize=maxx-4;
    char *buffer=malloc(bufsize);

    while(1)
    {
        bzero(buffer,bufsize);
        wrefresh(top);
        wrefresh(bottom);

        //Receive message from server
        read(sockfd,buffer,bufsize);
        if(strlen(buffer)==0){

            finished_chat = 1;
            serverDownflag=1;
            // Clean up
            endwin();      
            pthread_mutex_destroy(&func_mut_th);
            pthread_exit(NULL);
            close(sockfd);

        }    
        // write to logs
        char* tok;
        char buffer1[100];
        strcpy(buffer1,buffer);
         tok= strtok(buffer1,":");
        //Print on own terminal
        if(strcmp(tok,toSend)==0)
        	mvwprintw(top,line,3,buffer);
        else
        	mvwprintw(top,line,35,buffer);
       // writetochatlog(buffer,1, tok);
        // scroll the top if the line number exceed height
             pthread_mutex_lock (&func_mut_th);

        if(line==maxy/2-2)
            scroll(top);

        else
            line++;

        // scroll the bottom if the line number exceed height
        if(input==maxy/2-2)
            scroll(bottom);
        else
        	input++;
            

        pthread_mutex_unlock (&func_mut_th);
    }
}