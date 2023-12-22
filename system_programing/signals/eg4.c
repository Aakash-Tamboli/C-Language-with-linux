#include<stdio.h>
#include<signal.h>
#include<unistd.h>
int main()
{
int processId;
printf("Enter process Id of the other program to which the signal is to be dispatched: ");
scanf("%d",&processId);
kill(processId,SIGUSR1);
printf("Signal sent to process with Id %d\n",processId);
return 0;
}
