#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<termio.h>

int selected=-1;
int nextSelection=1;


void goToXY(int,int);
void clearUp();
void clearLine(int,int);
void say(int,int,char *);
void errorExit(const char *);
void trapArrowKey();

void clearUp()
{
write(fileno(stdout),"\033[1J",4);
}

void clearLine(int row,int col)
{
goToXY(row,col);
write(fileno(stdout),"\033[2K",4);
}

void goToXY(int row,int column)
{
char buffer[10];
int x=sprintf(buffer,"\033[%d;%dH",row,column); // see VT-100 Escape sequen
write(fileno(stdout),buffer,x);
}

void say(int row,int column,char *str)
{
goToXY(row,column);
write(fileno(stdout),str,strlen(str));
}

void errorExit(const char *ptr)
{
printf("%s\n",ptr);
exit(0);
}

void trapArrowKey()
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
a=getchar();
if(a==27) // it means Special key pressed arrow or function  Attention Don't confuse with special symbols special key means arrow or function f1,f2.. keys
{
printf("Special key tapped\n");
b=getchar();
c=getchar();
if(b==91 && c==65)
{
nextSelection--;
if(nextSelection==0) nextSelection=3;
}
else if(b==91 && c==66)
{
nextSelection++;
if(nextSelection>3) nextSelection=1;
}
}
else if(a=='\n')
{
selected=10; // 10 is marker that user selected option
}
if(tcsetattr(STDIN_FILENO,0,&oldState)==-1)
{
errorExit("tcsetattr error\n");
}
}


int main()
{
while(1)
{
clearUp();
if(nextSelection==1) say(5,25,"[Add]");
else say(5,25,"Add");
if(nextSelection==2) say(6,25,"[Edit]");
else say(6,25,"Edit");
if(nextSelection==3) say(7,25,"[Delete]");
else say(7,25,"Delete");
trapArrowKey();
if(selected==10)
{
if(nextSelection==1) say(10,25,"Add Selected\n\n");
else if(nextSelection==2) say(10,25,"Edit Selected\n\n");
else if(nextSelection==3) say(10,25,"Delete Selected\n\n");
exit(0);
}
clearLine(5,15); 
clearLine(5,15);
clearLine(7,15); 
}
return 0;
}
