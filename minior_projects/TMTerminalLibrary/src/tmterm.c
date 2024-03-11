#ifndef __$__TMTERM_C__
#define __$__TMTERM_C__ 8080
#include<tmterm.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<termio.h>
#include<stdlib.h>


// Global Members starts

struct termios old_state;
struct termios new_state;

// Global Members ends


void clear()
{
write(fileno(stdout),"\033[2J",4); // see VT-100 Chart
/*
write(descriptor,string,no_of_character);
now might thinking why 4 ?
\033 is count as 1st character.
[ is count as 2nd character.
2 is count as 3rd character.
J is count as 4th character.
Aakash following link will help you to understand VT100 escape 
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
<ESC> represents the ASCII "escape" character, hence its hex is: 0x1B. that's why sir wrote \0331B
next '[2J' is Erases the screen with the background colour and moves the cursor to home.
*/
}

void clearLine(int row,int col)
{
goToXY(row,col);
write(fileno(stdout),"\033[2K",4);
}

void goToXY(int row,int column)
{
char buffer[10];
int x=sprintf(buffer,"\033[%d;%dH",row,column); // see VT-100 Escape sequence chard
write(fileno(stdout),buffer,x);
/*
we want to write
write(fileno(stdout),"\033[row;colH",no_of_characters)
- this [row;col is dynamic part that's why we used sprintf functionality
- another thing [row;col is Sets the cursor position where subsequent text will begin. If no row/column parameters are provided (ie. <ESC>[H), the cursor will move to the home position, at the upper left of the screen.
- sprintf and printf return no of character it writes.
*/
}

void say(int row,int column,char *str)
{
goToXY(row,column);
write(fileno(stdout),str,strlen(str));
}

void drawHorizontalLine(int row,int column1,int column2)
{
/*
1st way to done
goToXY(row,column1);
while(column1<=column2)
{
printf("\u2500"); // sir suggest search for unicode box characters.
column1++;
}
fflush(stdout);
*/

/*
while(column1<=column2)
{
say(row,column1,"\u2500"); // see unicode box characters.
column1++;
}
*/

char *str="\u2500";
goToXY(row,column1);
while(column1<=column2)
{
write(fileno(stdout),str,strlen(str)); // see unicode box characters.
column1++;
}

// UniCode Char link
https://en.wikipedia.org/wiki/Box-drawing_character
}
//--------------------------------------------------------------
// Assignment Starts


// hide and show input realted function starts

void error_exit(const char *ptr)
{
printf("%s\n",ptr);
exit(0);
}

void hide_input()
{
if(tcgetattr(STDIN_FILENO,&old_state)==-1)
{
error_exit("tcgetattr error\n");
}
new_state=old_state;
new_state.c_lflag=new_state.c_lflag & ~(ECHO | ICANON);
new_state.c_cc[VTIME]=0;
new_state.c_cc[VMIN]=1; // it means we just want to read one character
if(tcsetattr(STDIN_FILENO,0,&new_state)==-1)
{
error_exit("tcsetattr error\n");
}
}

void show_input()
{
if(tcsetattr(STDIN_FILENO,0,&old_state)==-1)
{
error_exit("tcsetattr error\n");
}
}

// hide and show input related function ends



unsigned int trapArrowKey()
{
/*
in docs i will clearly specify that if trapArrow key function returns
65 then up-arrow key
66 then down-arrow key
67 then right-arrow key
68 then left arrow key

and if it returns none of those 4 then it return corresponding key press ascii code

*/

unsigned result;
hide_blinking_cursor();
char a,b,c;
a=getchar();

/*

The ASCII values for the arrow keys are as follows:

Up arrow: 27, 91, 65
Down arrow: 27, 91, 66
Left arrow: 27, 91, 68
Right arrow: 27, 91, 67

In nutshell: if ascii code is 27 then 100% arrow key is pressed by user
if 91 and 65 then up-arrow key
if 91 and 66 then down-arrow key
if 91 and 68 then left arrow key
if 91 and 67 then right arrow key

*/


if(a==27) // it means Special key pressed arrow or function  Attention Don't confuse with special symbols special key means arrow or function f1,f2.. keys
{
b=getchar();
c=getchar();
result=c;
}
else
{
result=a;
}
show_blinking_cursor();
return result;
}

void drawVerticalLine(int column,int fromRow,int toRow) // column, from_row , to_row
{
goToXY(fromRow,column);
while(fromRow<=toRow)
{
say(fromRow,column,"\u2502");
fromRow++;
}
}


void drawDoubleVerticalLine(int column,int fromRow,int toRow) // column, from_row , to_row
{
goToXY(fromRow,column);
while(fromRow<=toRow)
{
say(fromRow,column,"\u2551");
fromRow++;
}
}


void drawDoubleHorizontalLine(int row,int column1,int column2)
{
char *str="\u2550";
goToXY(row,column1);
while(column1<=column2)
{
write(fileno(stdout),str,strlen(str)); // see unicode box characters.
column1++;
}
}

void drawBox(int upperLeftRow, int upperLeftColumn,int lowerRightRow,int lowerRightColumn)
{
char *topLeftCorner="\u250C";
char *topRightCorner="\u2510";
char *bottomLeftCorner="\u2514";
char *bottomRightCorner="\u2518";

// printing leftCorner
goToXY(upperLeftRow,upperLeftColumn);
write(fileno(stdout),topLeftCorner,strlen(topLeftCorner));

// printing left to right line
drawHorizontalLine(upperLeftRow,upperLeftColumn+1,lowerRightColumn-1);

// printing right Corner
goToXY(upperLeftRow,lowerRightColumn);
write(fileno(stdout),topRightCorner,strlen(topRightCorner));

// printing right corner to bottom right corner
drawVerticalLine(lowerRightColumn,upperLeftRow+1,lowerRightRow-1);

// printing topleft to bottom left
drawVerticalLine(upperLeftColumn,upperLeftRow+1,lowerRightRow-1);

// printing bottom left corner
goToXY(lowerRightRow,upperLeftColumn);
write(fileno(stdout),bottomLeftCorner,strlen(bottomLeftCorner));

// printing bottom left to bottom right
drawHorizontalLine(lowerRightRow,upperLeftColumn+1,lowerRightColumn-1);

// printing bottom right corner
goToXY(lowerRightRow,lowerRightColumn);
write(fileno(stdout),bottomRightCorner,strlen(bottomRightCorner));
}


void drawDoubleLineBox(int upperLeftRow, int upperLeftColumn,int lowerRightRow,int lowerRightColumn)
{
char *topLeftCorner="\u2554";
char *topRightCorner="\u2557";
char *bottomLeftCorner="\u255A";
char *bottomRightCorner="\u255D";

// printing leftCorner
goToXY(upperLeftRow,upperLeftColumn);
write(fileno(stdout),topLeftCorner,strlen(topLeftCorner));

// printing left to right line
drawDoubleHorizontalLine(upperLeftRow,upperLeftColumn+1,lowerRightColumn-1);

// printing right Corner
goToXY(upperLeftRow,lowerRightColumn);
write(fileno(stdout),topRightCorner,strlen(topRightCorner));

// printing right corner to bottom right corner
drawDoubleVerticalLine(lowerRightColumn,upperLeftRow+1,lowerRightRow-1);

// printing topleft to bottom left
drawDoubleVerticalLine(upperLeftColumn,upperLeftRow+1,lowerRightRow-1);

// printing bottom left corner
goToXY(lowerRightRow,upperLeftColumn);
write(fileno(stdout),bottomLeftCorner,strlen(bottomLeftCorner));
// printing bottom left to bottom right
drawDoubleHorizontalLine(lowerRightRow,upperLeftColumn+1,lowerRightColumn-1);
// printing bottom right corner
goToXY(lowerRightRow,lowerRightColumn);
write(fileno(stdout),bottomRightCorner,strlen(bottomRightCorner));
}


int get_password(char *ptr,int size) // -1 or 0
{
char *tmp=NULL;
tmp=(char *)malloc(size+1);
if(tmp==NULL)
{
return -1;
}
struct termios oldState,newState;
if(tcgetattr(STDIN_FILENO,&oldState)==-1)
{
free(tmp);
return -1;
}
newState=oldState;
newState.c_lflag=newState.c_lflag & ~ECHO;
if(tcsetattr(STDIN_FILENO,0,&newState)==-1)
{
free(tmp);
return -1;
}
fgets(tmp,size+1,stdin);
fflush(stdin);
tmp[strlen(tmp)-1]='\0';
strcpy(ptr,tmp);
free(tmp);
if(tcsetattr(STDIN_FILENO,0,&oldState)==-1)
{
free(tmp);
return -1;
}
return 0;
}

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
// black
// blue
// white
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


// Hiding Cursor functionality starts

void hide_blinking_cursor()
{
char hide_cursor[]="\033[?25l";
write(fileno(stdout),hide_cursor,strlen(hide_cursor));
}

void show_blinking_cursor()
{
char show_cursor[]="\033[?25h";
write(fileno(stdout),show_cursor,strlen(show_cursor));
}

// Hiding Cursor functionality ends


#endif
