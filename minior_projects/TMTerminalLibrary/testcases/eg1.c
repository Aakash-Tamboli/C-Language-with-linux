#include<tmterm.h>
int main()
{
clear();
drawVerticalLine(1,1,7);
drawDoubleVerticalLine(3,1,7);
drawBox(2,5,10,10); // Diagonal Points
drawDoubleHorizontalLine(7,20,60);
drawDoubleLineBox(14,10,20,20);
return 0;
}
