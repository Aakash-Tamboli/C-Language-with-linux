#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<termio.h>
#include<math.h>

// Menu Oriented Settings
int choices[20]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char options[20][12]={"Opt-1","Opt-2","Opt-3","Opt-4","Opt-5","Opt-6","Opt-7","Opt-8","Opt-9","Opt-10","Opt-11","Opt-12","Opt-13","Opt-14","Opt-15","Opt-16","Opt-17","Opt-18","Opt-19","Opt-20"};
int totalOptions=20;
int numberOfOptionsToBeDisplay=4;
int sp;
int ep;
int selected=-1;
int previousSelection=-1;
int currentSelection=0;



// Color Oriented Function start
// Visit For VT-100 specific

// https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm

void reset_color_settings()
{
char Reset[]="\033[0m";
write(fileno(stdout),Reset,strlen(Reset));
}

void set_foreground_color(char c)
{
char Bright[10]="\033[1m";

char Dim[10]="\033[2m";
char Underscore[10]="\033[4m";
char Blink[10]="\033[5m";
char Reverse[10]="\033[7m";
char Hidden[10]="\033[8m";


char Black[10]="\033[30m";
char Red[10]="\033[31m";
char Green[10]="\033[32m";
char Yellow[10]="\033[33m";
char Blue[10]="\033[34m";
char Magenta[10]="\033[35m";
char Cyan[10]="\033[36m";
char White[10]="\033[37m";


if(c=='b') write(fileno(stdout),Black,strlen(Black));
else if(c=='n') write(fileno(stdout),Blue,strlen(Blue));
else if(c=='w') write(fileno(stdout),White,strlen(White));
// reset will be decide later on
}

void set_background_color(char c)
{
char Bright[10]="\033[1m";
char Dim[10]="\033[2m";
char Underscore[10]="\033[4m";
char Blink[10]="\033[5m";
char Reverse[10]="\033[7m";
char Hidden[10]="\033[8m";


char Black[10]="\033[40m";
char Red[10]="\033[41m";
char Green[10]="\033[42m";
char Yellow[10]="\033[43m";
char Blue[10]="\033[44m";
char Magenta[10]="\033[45m";
char Cyan[10]="\033[46m";
char White[10]="\033[47m";

if(c=='b') write(fileno(stdout),Black,strlen(Black));
else if(c=='n') write(fileno(stdout),Blue,strlen(Blue));
else if(c=='w') write(fileno(stdout),White,strlen(White));
// reset will be decide later on
}

void set_color(char foreground,char background)
{
set_foreground_color(foreground);
set_background_color(background);
}


// color Oriend Function ends


void goToXY(int,int);
void clearUp();
void clearLine(int,int);
void say(int,int,char *,int);
void errorExit(const char *);
void trapArrowKey();
void changeColor();
void displayMenuOptions();

int main()
{
int i;
char selectedOption[30];
sp=0;
ep=3;
while(1)
{
clearUp();
displayMenuOptions();
trapArrowKey();
if(selected==10)
{
for(i=0;i<totalOptions;i++) 
{
if(choices[i]==1)
{
sprintf(selectedOption,"%s selected\n",options[i]);
say(10,25,selectedOption,1);
}
}
exit(0);
}
for(i=0;i<numberOfOptionsToBeDisplay;i++) clearLine(i+5,15); 
}
return 0;
}




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
set_background_color('b');
set_background_color('w');
write(fileno(stdout),str,strlen(str));
reset_color_settings();
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
// printf("Special key tapped\n");
b=getchar();
c=getchar();
if(b==91 && c==65)
{
previousSelection=currentSelection;
choices[currentSelection]=0;
currentSelection--;
if(currentSelection<0) currentSelection=totalOptions-1;
choices[currentSelection]=1;
}
else if(b==91 && c==66)
{
previousSelection=currentSelection;
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
int counter,i;
// char tmp[50];
if(currentSelection==0)
{
sp=0;
ep=3;
}
else if(currentSelection==totalOptions-1)
{
ep=currentSelection;
sp=ep-3;
}
else if(currentSelection>ep)
{
sp=sp+1;
ep=ep+1;
}
else if(currentSelection<sp)
{
ep=ep-1;
sp=sp-1;
}

i=sp;

counter=0;
while(counter<numberOfOptionsToBeDisplay)
{
if(choices[i]==1)
{
// sprintf(tmp,"i=%d , choices[%d]=%d",counter,i,choices[i]);
say(5+counter,25,options[i],1);
// say(5+counter,55,tmp,1);
}
else
{
// sprintf(tmp,"i=%d , choices[%d]=%d",counter,i,choices[i]);
say(5+counter,25,options[i],0);
// say(5+counter,55,tmp,1);
}
i++;
counter++;
}
}
