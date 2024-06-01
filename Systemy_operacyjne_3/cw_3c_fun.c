#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#define PATH "./"
#define N 100

#include <unistd.h> //pause()
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>  //printf() & perror()
#include <signal.h> //signal() & kill()
#include <string.h> //strcmp()
#include <stdlib.h> //exit()

/*
Program pomocniczy do podpunktu c). Tworzy trzy procesy potomne
i czeka na ich zakończenie.
Autor: Michał Śliż                   Kraków 12.04.2023
*/

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Błąd. Podano za mało argumentów wywołania programu. "
               "Podaj argumenty w postaci make run-cw_3c ARGS='nazwa_uruchamianego_programu numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    }
    else if (argc > 4)
    {
        printf("Błąd. Podano za dużo argumentów wywołania programu. "
               "Podaj argumenty w postaci make run-cw_3c ARGS='nazwa_uruchamianego_programu numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    }
    int i;
    int fork_value;
    int child_status; // status procesu potomnego
    int wait_status;
    int signal_num = strtoll(argv[2], NULL, 10);
    char path_name[N]; // nazwa uruchamianego programu
    sprintf(path_name, "%s%s", PATH, argv[1]);

    for (i = 0; i < 3; ++i)
    {
        fork_value = fork();

        if (fork_value == -1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);

        } // akcja dla procesu potomnego
        else if (fork_value == 0)
        {
            // uruchomienie programu z podpunktu a)
            if (execlp(path_name, path_name, argv[2], argv[3], NULL) == -1)
            {
                perror("execl error");
                exit(EXIT_FAILURE);
            }
        }
    }

    // czkekanie na wszystkie procesy potomne
    while (1)
    {
        // ustawienie ignorowania sygnału
        signal(signal_num, SIG_IGN);

        wait_status = wait(&child_status);
        if (wait_status == -1) {
            // printf("Proces zaczekał na wszystkie swoje procesy potomne\n");
            break;
        }

        printf("PID: %d, status zakończenia: %d\n", wait_status, child_status);

        if (WIFSIGNALED(child_status))
        {
            printf("Proces o PID %d został zakończony przez sygnał %s, któremu odpowiada numer %d\n", wait_status, strsignal(WTERMSIG(child_status)), WTERMSIG(child_status));
        }
    }

    exit(EXIT_SUCCESS);
}
