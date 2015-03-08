/**
 * Daemon C program to insert a kernel module to print 'Hello World!' if not inserted
 * and delete it if already present every 5 seconds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[])
{
    pid_t process_id = 0;
    pid_t sid        = 0;
    FILE *fd;
    char buf[16];

    process_id = fork();
    if (process_id < 0)         /*Indication of fork() failure*/
    {
        printf("fork failed!\n");
        exit(1);
    }
    if (process_id > 0)         /*PARENT PROCESS - Need to kill it*/
    {
        printf("process_id of child process %d \n", process_id);
        exit(0);
    }

    umask(0);                   /*unmask the file mode*/
    sid = setsid();             /*set new session*/
    if (sid < 0)
    {
        exit(1);
    }
    chdir("/");                 /*Change the current working directory to root*/
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        fd = popen("lsmod | grep hello", "r");
        if (fread(buf, 1, sizeof (buf), fd) > 0) 
        {   /*if there is some result the module must be loaded, so remove it*/
            system("/sbin/rmmod ~/Daemon/hello.ko");
        }
        else
        {
            /*Insert module*/
            system("/sbin/insmod ~/Daemon/hello.ko");
        }
        sleep(5);
    }
    return (0);
}
