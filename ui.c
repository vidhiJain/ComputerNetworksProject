#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "security.c"
#include <unistd.h>
#include <fcntl.h>
 #define RESET "\e[m" 
 #define MAKE_GREEN "\e[32m" 
 #define MAKE_BLUE "\e[36m" 

void green()
{
	printf(MAKE_GREEN"");
}
void blue()
{
	printf(MAKE_BLUE"");
}
void reset()
{
	printf(RESET"");
}
char desid[17];
int  listdir() {
        int len;
        struct dirent *pDirent;
        DIR *pDir;
        pDir = opendir ("data");
        if (pDir == NULL) {
            printf ("Cannot open directory 'data'\n");
            return 0;
        }

        while ((pDirent = readdir(pDir)) != NULL) {
        	if(!(strcmp(pDirent->d_name,".")==0 || strcmp(pDirent->d_name,"..")==0))
            printf ("%s ", pDirent->d_name);
        }
        closedir (pDir);
        return 1;
    }
char *home()
{
	system("clear");
	reset();
	char name[30]="Test";

	//getname(name);
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
   	// 			char temp[30];
				//  scanf("%s",temp);
				//  char temp2[30]="data/"; 	
				// strcat(temp2,temp);
				// file1 = fopen( temp2 , "w+"); // take user id from server and write to file
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
								 FILE * file;
								 char temp1[30]="data/"; 	
								 strcat(temp1,temp);
								 file = fopen( temp1 , "r");
								 if (file) {
								 	notopen=0;
								 	char ch;
								 	system("clear");
								 //	int color=0;
								 	int id_index=0;
								 	
								 	for(;id_index<15;id_index++){
								 		ch = fgetc(file);
								 		desid[id_index]=ch;
								 	}
								 	ch = fgetc(file);
								 	desid[17]='\0';
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
   								 scanf("%s",temp);
   								//writetochatlog(temp,"You","Kuchtohtime",0,temp1);
   								 return desid;
								 	}
									else
								 		printf("Please enter correct chatname \n");
									}

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
					home(); // recursive calls!! dangeorus
				}
				else
					printf("Please enter correct chatname \n");
				break;
	}
return NULL;
}

int writetochatlog(char *message,int flag, char* filename)
{
								 FILE * fp;
								 char temp1[30]="data/"; 

								 char name [50];
   									name[0]='|';
   									name[2]='\0';
   									if(flag==0) 
   									{name[1]='^'; //you
   								//strcpy(temp1,filename);
   							}	
   									else
   									{ name[1]='*';
   								
   							}
   									//strcat(name,from);
								 	strcat(temp1,filename);
								// printf("Opening %s\n",temp1);
								 fp = fopen( temp1 , "a+");
								 //int filedesc = open(temp1, O_WRONLY | O_APPEND);
								 if (fp == NULL) {
        								 printf("LogFile Error");
         								return 0;
      }
   		
   		//encrypt(name);
   		//encrypt(time);
   		//encrypt(message);
      fprintf(fp, "\n");
     // char str[2]="\n";
      // write(filedesc, str , sizeof(str) );
      fprintf(fp, "%s",name);
     // write(filedesc, name , sizeof(name) );
      //printf("%s",name);
      //fprintf(fp, "[%s]:",time);
      fprintf(fp, "%s",message);
      // write(filedesc, message , sizeof(message) );
      //printf("%s",message);
      fprintf(fp, "\n");

   
      /* close the file */
      fclose(fp);



}
int login(char * user)
{
	static const char * p[30]={"Rishabh","Vidhi","Mohit","User1","User2","User3","User4"};
	static const char  *u[30]={"Rishabh","Vidhi","Mohit","User1","User2","User3","User4"};
	//char user[30];
	char * pass;
	
	printf("Enter Username:\n");
	scanf("%s",user);
	//printf("Enter Password:\n");
	//scanf("%s",pass);
	char * prompt="Enter Password:\n";
	pass=getpass(prompt);
	int li=0;
	for(;li<7;li++)
	{
		if(strcmp(u[li],user)==0 && strcmp(p[li],pass)==0)
			return 1;
	}
	printf("Wrong Username/Password\n");
	 return 0;

}
// int  main()
// {
// 	// connect to server
// 	// system("clear");
// 	// while(!login());

// 	// home();
// 	char msg[20]="You: Blah";
// 	char name[20]="Rahul";
// 	writetochatlog(msg,0,name);
// 	char msg1[20]="Rahul:Blaher";
	
// 	writetochatlog(msg1,1,name);

// 	return 0;
// }