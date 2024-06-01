#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
    pid_t uid, gid, pid, ppid, pgid;

    uid = getuid();
    gid = getgid();
    pid = getpid();
    ppid = getppid();
    pgid = __getpgid(0);

    printf("Identyfikatory procesu:\n");
    printf("UID = %d, GID = %d, PID = %d, ", uid, gid, pid);
    printf("PPID = %d, PGID = %d\n", ppid, pgid);


    return 0;
}