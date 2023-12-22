#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main()
{
int x,result,data;
union sigval signalValue;
printf("Enter Process id of the process to which the signal is to be dispatched: ");
scanf("%d",&x);
printf("Enter data to send: ");
scanf("%d",&data);
signalValue.sival_int=data;
sigqueue(x,SIGUSR1,signalValue);
return 0;
}
