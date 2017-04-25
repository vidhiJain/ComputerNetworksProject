#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE * FindReccomendations(char * keyword)
{
	char command[40]="grep ";
	strcat(command,keyword);
	strcat(command," ques.txt > rec.txt");
	system(command);
	FILE *file = fopen ( "rec.txt", "r" );
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

int main()
{
	char keyword[30]="this";
	//GetReccomendations(keyword);
	return 0;
}