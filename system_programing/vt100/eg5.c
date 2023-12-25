#include<stdio.h>
#include<unistd.h>
int main()
{
int fileDescriptor;
fileDescriptor=fileno(stdout); // file is function which return a descriptor
write(fileDescriptor,"Cool",4); // write(descriptor,string,no_of_character_you_want_to_write);
sleep(3);
return 0;
}
