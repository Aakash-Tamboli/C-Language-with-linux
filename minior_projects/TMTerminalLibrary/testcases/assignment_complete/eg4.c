#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<termio.h>

// Menu Oriented Settings
int choices[3]={1,0,0};
char options[3][6]={"Add","Edit","Delete"};
int totalOptions=3;
int selected=-1;
int currentSelection=0;


// Color Oriented Settings
char bright[]="\033[1m";
char bck[]="\033[40m";
char fore[]="\033[37m";
char reset[]="\033[0m";



void goToXY(int,int);
void clearUp();
void clearLine(int,int);
void say(int,int,char *,int);
void errorExit(const char *);
void trapArrowKey();
void changeColor();

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

void say(int row,int column,char *str,int paint)
{
goToXY(row,column);
if(paint)
{
write(fileno(stdout),bright,strlen(bright));
write(fileno(stdout),bck,strlen(bck));
write(fileno(stdout),fore,strlen(fore));
write(fileno(stdout),str,strlen(str));
write(fileno(stdout),reset,strlen(reset));
return;
}
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
choices[currentSelection]=0;
currentSelection--;
if(currentSelection<0) currentSelection=totalOptions-1;
choices[currentSelection]=1;
}
else if(b==91 && c==66)
{
choices[currentSelection]=0;
currentSelection++;
if(currentSelection>(totalOptions-1)) currentSelection=0;
choices[currentSelection]=1;
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


void displayMenuOptions()
{
int i;
for(i=0;i<totalOptions;i++)
{
if(choices[i]==1)
{
say(5+i,25,options[i],1);
}
else
{
say(5+i,25,options[i],0);
}
}
}


int main()
{
int i;
while(1)
{
clearUp();
displayMenuOptions();
trapArrowKey();
if(selected==10)
{
for(i=0;i<totalOptions;i++) if(choices[i]==1) say(10,25,options[i],1);
exit(0);
}
clearLine(5,15); 
clearLine(5,15);
clearLine(7,15); 
}
return 0;
}
