#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "print.h" // nagłówek z deklaracją funkcji print()


int main()
{
    print(); // wywołanie funkcji print() zdefiniowanej poniżej

    return 0;
}


void print(void)
{
    printf("\nIdentyfikatory procesu potomonego:\n");
    printf("UID = %d, GID = %d, PID = %d, ", getuid(), getgid(), getpid());
    printf("PPID = %d, PGID = %d\n", getppid(), getpgrp());
}
