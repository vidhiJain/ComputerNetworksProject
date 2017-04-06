#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

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
	reset();
	char name[30]="Test";
	//getname(name);
	printf("Welcome %s\n",name);
	printf("Choose One of the following options:\n");
	printf("1.Start a new chat \t");

	printf("2.View an old chat \t");
	printf("3.*3rd option kya tha?*\n");
	int o=0;
	scanf("%d",&o);
	switch(o)
	{
		case 1:// show list of all contacts
				break;
		case 2: system("clear");
				printf("Please Name the chat you want to open \n");
				listdir();
				printf("\n");
		        char temp[30];
		        int notopen=1;
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
								 	char desid[17];
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
   								// scanf("%s",temp);
   								// writetochatlog(temp,"You","Kuchtohtime",0,temp1);
								 	}
									else
								 		printf("Please enter correct chatname \n");
									}

				break;
		case 3:
				break;
	}
return NULL;
}

int writetochatlog(char *message,char*from,char* time,int flag, char* filename)
{
								 FILE * fp;
								 char temp1[30]="data/"; 
								 char name [50];
   									name[0]='|';
   									name[2]='\0';
   									if(flag==0) 
   									{name[1]='^'; //you
   								strcpy(temp1,filename);
   							}	
   									else
   									{ name[1]='*';
   								strcat(temp1,filename);
   							}
   									strcat(name,from);
								 	
								 
								 fp = fopen( temp1 , "a+");
								 if (fp == NULL) {
        								 printf("LogFile Error");
         								return 0;
      }
   		
      fprintf(fp, "\n");
      fprintf(fp, "%s",name);
      fprintf(fp, "[%s]:",time);
      fprintf(fp, "%s",message);
      fprintf(fp, "\n");

   
      /* close the file */
      fclose(fp);



}
int  main()
{
	// connect to server
	home();
	return 0;
}