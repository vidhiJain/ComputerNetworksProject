#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE * FindReccomendations(char * keyword)
{
   char command[1000]="grep ";
   //strcat(command,keyword);
   //puts("Idhar aaya");
   char * tok;
   tok=strtok(keyword," ");
   strcat(command,tok);
   strcat(command," ques.txt");
   tok=strtok(NULL," ");
   while(tok!=NULL)
   {
     // puts("Idhar aaya2");
      
      strcat(command,"| grep ");
      strcat(command,tok);
      tok=strtok(NULL," ");
   }
 //  puts("Idhar bhi aaya2");
   strcat(command," > rec.txt");
   system(command);
   FILE *file = fopen ( "rec.txt", "r" );
   return file;
}

int GetReccomendations(char * buffer, FILE *file)
{
   if ( file != NULL )
   {
      
      while ( fgets ( buffer, sizeof(buffer), file ) != NULL ) 
      {
         return 1;
      }
      fclose ( file );
      return 0;
   }
   else
   {
      // print error?

  }
}

int GetAns(char key, char * buffer)
{
   static const char filename[] = "ans.txt";
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
     
      while ( fgets ( buffer, sizeof(buffer), file ) != NULL ) 
      {
         
         if(buffer[0]==key)
            return 1;
               
               }
      fclose ( file );
   }
   else
   {
      perror ( filename ); 
   }
   return 0;
}
