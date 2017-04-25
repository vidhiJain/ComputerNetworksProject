#include <stdio.h>
#include <string.h>
int a=2,b=3,c=4,d=2;


void encrypt(char * s)
{
	int len=strlen(s);
	int i=0;
	while(i<len)
	{
		s[i]-=a+b-c*d;
		i++;
	}
}
void decrypt(char * s)
{
	int len=strlen(s);
	int i=0;
	while(i<len)
	{
		s[i]+=a+b-c*d;
		i++;
	}
}
char encryptchar(char  s)
{
	
		s-=a+b-c*d;
	return s;
}
char decryptchar(char  s)
{
	
	
		s+=a+b-c*d;
		
		return s;
	
}

// int main()
// {
// 	char s[]="Testing";

// 	encrypt(s);
// 	printf("%s\n",s );
// 	decrypt(s);
// 	printf("%s\n",s );
// 	printf("%c\n", decryptchar(encryptchar('a')));
// }