#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

void signalHandler(int signalNumber,siginfo_t *information,void *extra)
{
int data=information->si_value.sival_int;
printf("Signal Handler got called and recived %d as data\n",data);
exit(0);
}

int main()
{
int result;
struct sigaction action;
sigset_t setOfSignals;
sigemptyset(&setOfSignals); // initializing setOfSignals to an empty state
action.sa_flags=SA_SIGINFO; // all raised signals will be queued means no signal will be drop.
action.sa_sigaction=signalHandler;
result=sigaction(SIGUSR1,&action,NULL);
if(result!=1)
{
printf("Signal Handler for user defined	1 type has been set\n");
}
else
{
printf("Unable to set signal handler for user defined 1 type signals\n");
return 0;
}
while(1)
{
printf("I am going to sleep\n");
sleep(5);
printf("I am wakeup\n");
}
return 0;
}
