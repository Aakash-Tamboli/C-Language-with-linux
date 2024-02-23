#include<stdio.h>
#include<termios.h>
#include<stdlib.h>
#include<unistd.h>
void errorExit(const char *ptr)
{
printf("%s\n",ptr);
exit(0);
}
int main()
{
struct termios oldState,newState;
if(tcgetattr(STDIN_FILENO,&oldState)==-1)
{
errorExit("tcgetattr error\n");
}
newState=oldState;
newState.c_lflag=newState.c_lflag & ~(ECHO | ICANON);
newState.c_cc[VTIME]=0;
newState.c_cc[VMIN]=1; // it means we just want to read one character
if(tcsetattr(STDIN_FILENO,0,&newState)==-1)
{
errorExit("tcsetattr error\n");
}
char a,b,c;
printf("Tap a key\n\n");
a=getchar();
if(a==27) // it means Special key pressed arrow or function  Attention Don't confuse with special symbols special key means arrow or function f1,f2.. keys
{
printf("Special key tapped\n");
b=getchar();
c=getchar();
printf("27 followed by %d and %d\n",b,c);
}
else
{
printf("Non Special key %d %c\n",a,a);
}
if(tcsetattr(STDIN_FILENO,0,&oldState)==-1)
{
errorExit("tcsetattr error\n");
}
return 0;
}
