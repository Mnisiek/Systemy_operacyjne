#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


pid_t macierzysty_pid; // zmienna globalna


int main()
{
    pid_t uid, gid, pid, ppid, pgid;
    int i;
    int fork_value;

    uid = getuid();
    gid = getgid();
    pid = getpid();
    ppid = getppid();
    pgid = __getpgid(0);

    printf("Identyfikatory procesu macierzystego:\n");
    printf("UID = %d, GID = %d, PID = %d, ", uid, gid, pid);
    printf("PPID = %d, PGID = %d\n", ppid, pgid);
    fflush(stdout);

    for (i = 0; i < 3; ++i) {

        fork_value = fork();

        if (fork_value == -1) {
            perror("fork error");
            exit(1);
        }
        else if (fork_value == 0)
        {
            sleep(5); // proces potomny jest wstrzymany na 5 sekund
            
            uid = getuid();
            gid = getgid();
            pid = getpid();
            ppid = getppid();
            pgid = __getpgid(0);

            printf("Identyfikatory procesu potomnego:\n");
            printf("UID = %d, GID = %d, PID = %d, ", uid, gid, pid);
            printf("PPID = %d, PGID = %d\n", ppid, pgid);
            fflush(stdout);
        }
        else if (fork_value > 0) {
            ;
        }

        if (getpid() == macierzysty_pid) {
            sleep(2);
    }
    
    }

    return 0;
}