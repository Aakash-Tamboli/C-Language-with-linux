#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

pid_t p;

void child_quit_signal_handler(int signalNumber)
{
printf("Child Quit Signal Got Called\n");
}

void parent_quit_signal_handler(int signalNumber)
{
printf("Parent Quit Signal Got Called\n");
}

int main()
{
p=fork();
if(p==0)
{
signal(SIGQUIT,child_quit_signal_handler);
printf("Child: I am going to sleep for 100 seconds\n");
sleep(100);
printf("I was Interupted in my sleep\n"); // if this line got see means parent raise/ calling kill function a signal to child
}
else
{
printf("%d\n",p); // process ID of Child
signal(SIGQUIT,parent_quit_signal_handler);
printf("Parent : I am going to sleep for 10 seconds\n");
sleep(5);
kill(p,SIGQUIT);
printf("Parent : I am waiting for child to end\n");
wait(NULL);
printf("Parent : Child has ended, Now I am going to ends\n");
}
return 0;
}
