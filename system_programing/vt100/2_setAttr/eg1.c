#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<termios.h>

void errorExit(const char *ptr)
{
printf("%s\n",ptr);
exit(1);
}

int main()
{
char a[21];
struct termios old_state,new_state;
if(tcgetattr(STDIN_FILENO,&old_state)==-1)
{
errorExit("Unable to get info using tcgetattr\n");
}
new_state=old_state;
new_state.c_lflag=new_state.c_lflag & ~ECHO;
if(tcsetattr(STDIN_FILENO,0,&new_state)==-1)
{
errorExit("Unable to set using tcsetattr\n");
}
printf("Enter Password: ");
scanf("%s",a); // you can use fgets if password having space, our objective is to learn passoword should not visible at terminal
if(tcsetattr(STDIN_FILENO,0,&old_state)==-1)
{
printf("Unable to set using tcsettr it means you have to change terminal environment other wise whatever you typed it won't visible\n");
}
printf("\n\nPassword: [%s]\n",a);
return 0;
}
