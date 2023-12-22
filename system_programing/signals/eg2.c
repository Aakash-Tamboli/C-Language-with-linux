#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

pid_t p;

void child_quit_signal_handler(int signalNumber)
{
printf("Child Quit Signal Got Called, hence I am goint to end child process\n");
abort(); // this will end the child process.
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
printf("I am about to end cleanly no one interupted my sleep\n"); // if you comment abort() function then this line will be appear. otherwise child handler will terminat the process because of abort function. // remeber wait() function should be called
}
else
{
printf("%d\n",p); // process ID of Child
signal(SIGQUIT,parent_quit_signal_handler);
printf("Parent : I am going to sleep for 10 seconds\n");
sleep(5);
kill(p,SIGQUIT);
printf("Parent : I am waiting for child to end\n");
wait(NULL); // v.v.v.v imp because it will clean all the resources used by child process.
printf("Parent : Child has ended, Now I am going to ends\n");
}
return 0;
}
