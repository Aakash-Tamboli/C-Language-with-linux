#ifndef __$__TMTERM_H__
#define __$__TMTERM_H__ 9090
void clear();
void clearLine(int,int);
void goToXY(int,int);
void say(int,int,char *);
void drawHorizontalLine(int,int,int);
void drawVerticalLine(int,int,int);
void drawDoubleVerticalLine(int,int,int);
void drawDoubleHorizontalLine(int,int,int);
void drawBox(int,int,int,int);
void drawDoubleLineBox(int,int,int,int);
void errorExit(const char *);
unsigned int trapArrowKey();
int get_password(char *,int);
void reset_color_settings();
void set_foreground_color(char);
void set_background_color(char);
void set_color(char,char);
void error_exit(const char *);
void hide_blinking_cursor();
void show_blinking_cursor();
#endif
