#ifndef __$__TMTERM_H__
#define __$__TMTERM_H__ 9090
void clear();
void goToXY(int,int);
void say(int,int,char *);
void drawHorizontalLine(int,int,int);
void drawVerticalLine(int,int,int);
void drawDoubleVerticalLine(int,int,int);
void drawDoubleHorizontalLine(int,int,int);
void drawBox(int,int,int,int);
void drawDoubleLineBox(int,int,int,int);
int get_password(char *,int);
#endif
