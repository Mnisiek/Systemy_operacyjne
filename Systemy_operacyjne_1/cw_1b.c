#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


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
            while (wait(NULL) > 1) // proces macierzysty czeka
                ;           // na wszystkie swoje procesy potomne

            printf("Proces %d zaczekał na proces potomny %d\n",
            pid, fork_value);
            fflush(stdout);
        }
    }

    return 0;
}