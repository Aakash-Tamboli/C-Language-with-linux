#include<tmterm.h>
#include<stdio.h>
int main()
{
char pws[21];
int succ;
printf("Enter Password:");
succ=get_password(pws,21);
if(succ==0) printf("\nPassword is: [%s]\n",pws);
return 0;
}
