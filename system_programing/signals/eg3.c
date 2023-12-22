#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void signalHandler(int signalNumber)
{
printf("Signal Number: %d git raised\n",signalNumber);
exit(0);
}

int main()
{
signal(SIGUSR1,signalHandler);
while(1)
{
printf("I am goind to sleep\n");
sleep(5);
printf("I am awake\n");
}
return 0;
}
