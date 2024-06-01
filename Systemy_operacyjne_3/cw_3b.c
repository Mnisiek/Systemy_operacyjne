/*
Uruchomić powyższy program poprzez funkcję execlp w procesie potomnym innego
procesu (z użyciem funkcji fork) i wysyłać do niego sygnały poprzez funkcję syste-
mową kill z procesu macierzystego. ! Uwaga: Przed wysłaniem sygnału sprawdzić,
czy proces istnieje (patrz podrozdział 3.2). Proces macierzysty powinien zaczekać
na zakończenie swojego potomka przy pomocy funkcji wait, a następnie wypisać
jego PID i status zakończenia zwracane przez tę funkcję. W przypadku zakończenia
procesu potomnego przez sygnał powinien wypisać dodatkowo numer tego sygnału
zawarty w statusie zakończenia, a także jego nazwę (użyć funkcji strsignal). Do
wyłuskania numeru sygnału ze statusu zakończenia użyć makr opisanych w podroz-
dziale 2.4.

Podpunkt (b): dwa programy - pierwszy w procesie potomnym uruchamia program
z podpunktu (a), po czym z procesu macierzystego wysyła do niego sygnał;
w procesie macierzystym przed wywołaniem funkcji kill() wstawić funkcję sleep()
z krótkim czasem uśpienia.

Numer sygnału oraz opcje jego obsługi we wszystkich powyższych programach przeka-
zywać za pomocą argumentów wywołania programu – sprawdzać ich liczbę i wypisywać
odpowiedni komunikat w przypadku błędnego uruchomienia (ze wskazówską poprawnego
uruchomienia).
*/

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
Program uruchamia w procesie potomnym pierwszy program (funkcja execlp()), a następnie wysyła do niego sygnały (funkcja kill()).
Autor: Michał Śliż                   Kraków 12.04.2023
*/

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Błąd. Podano za mało argumentów wywołania programu. "
        "Podaj argumenty w postaci make run-cw_3b ARGS='nazwa_uruchamianego_programu numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    } else if (argc > 4) {
        printf("Błąd. Podano za dużo argumentów wywołania programu. "
        "Podaj argumenty w postaci make run-cw_3b ARGS='nazwa_uruchamianego_programu numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    }

    int fork_value;
    int child_status; // status procesu potomnego
    int signal_num = strtoll(argv[2], NULL, 10);
    char path_name[N]; // nazwa uruchamianego programu
    sprintf(path_name, "%s%s", PATH, argv[1]);

    fork_value = fork();

    if (fork_value == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);

    // akcja dla procesu potomnego
    } else if (fork_value == 0) {
        // uruchomienie programu z podpunktu a)
        if (execlp(path_name, path_name, argv[2], argv[3], NULL) == -1) {
            perror("execl error");
			exit(EXIT_FAILURE);
        }
    }
    // akcja dla procesu macierzystego
    else if (fork_value > 0) {
        // sprawdzenie czy proces intnieje
        if (kill(fork_value, 0) == -1) {
            perror("kill error");
            exit(EXIT_FAILURE);
        }
        sleep(2);

        // wysłanie sygnału do procesu potomnego
        if (kill(fork_value, signal_num) == -1) {
            perror("kill error");
            exit(EXIT_FAILURE);
        }

        // proces macierzysty czeka na swój proces potomny
        if (waitpid(fork_value, &child_status, WUNTRACED) == -1) {
            perror("waitpid error");
            exit(EXIT_FAILURE);
        }

        printf("PID procesu potomnego: %d\n", fork_value);
        printf("%d\n", child_status);

        if (WIFSIGNALED(child_status)) {
            printf("Proces o PID: %d został zakończony przez sygnał %s, któremu odpowiada numer %d\n", fork_value, strsignal(WTERMSIG(child_status)), WTERMSIG(child_status));

        }
        else {
            printf("Proces o PID: %d zakończył się i zwrócił status: %d\n", fork_value, WEXITSTATUS(child_status));
        }
    }

    exit(EXIT_SUCCESS);
    }
    