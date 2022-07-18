#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


extern long long int __sangjun;
int main()
{
	char input[0x30];
	scanf("%s",input);
	

	__sangjun=1;
	if(input[0]=='s')
	{
		if(input[1]=='a')
			printf("done");
	}
}
