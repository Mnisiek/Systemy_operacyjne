#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
Program wypisuje identyfikatory procesów potomnych przy użyciu
drugiego programu uruchamianego przez funkcję execlp().
Autor: Michał Śliż                   Kraków 29.03.2023
*/

int main(int argc, char* argv[])
{
    int i; // zmienna do iteracji w pętli
    int fork_value; // wartość zwracana przez funkcję fork()
    int child_status; // status procesu potomnego

    // obsługa błędów wywołania programu
    if (argc != 3) {
        printf("Niepoprawna liczba argumentów. Sprawdź plik Makefile. Uruchom program wpisując:"
        "make run-cw_2\n");
        exit(EXIT_FAILURE);
    }

    printf("\nIdentyfikatory procesu macierzystego:\n");
    printf("UID = %d, GID = %d, PID = %d, ", getuid(), getgid(), getpid());
    printf("PPID = %d, PGID = %d\n", getppid(), getpgrp());
    fflush(stdout);

    // trzykrotne wywołanie funkcji systemowej fork(), która tworzy kopię procesu
    for (i = 0; i < 3; ++i) {

        fork_value = fork();

        // obsługa błędu wywołania funkcji fork()
        if (fork_value == -1) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }

        // akcja dla procesu potomnego
        else if (fork_value == 0)
        {
            if (execlp(argv[1], argv[2], NULL) == -1) {
                perror("execlp error");
                exit(EXIT_FAILURE);
            }
        }

        // akcja dla procesu macierzystego
        else if (fork_value > 0) {
            
            // obsługa błędu wywołania funkcji waitpid()
            if (waitpid(fork_value, &child_status, WUNTRACED) == -1) {
                perror("waitpid error");
                exit(EXIT_FAILURE);
        }
            // wypisanie komunikatu
            // printf("Proces %d zaczekał na proces potomny %d\n",
            // getpid(), fork_value);
            // fflush(stdout);
        }
    }

    return 0;
}