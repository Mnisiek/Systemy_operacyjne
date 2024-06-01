/*
W procesie macierzystym utworzyć proces potomny i sprawić, aby stał się liderem
nowej grupy procesów (funkcja setpgid), a następnie poprzez funkcję execlp uru-
chomić w nim program tworzący kilka procesów potomnych, które (poprzez execlp)
wykonują program do obsługi sygnałów z punktu(a). Z pierwszego procesu macie-
rzystego wysyłać sygnały do całej grupy procesów potomnych po uprzednim spraw-
dzeniu jej istnienia. Identyfikator tej grupy procesów uzyskać przy pomocy funk-
cji getpgid. Proces będący liderem grupy procesów niech ignoruje sygnały, a na
końcu czeka na zakończenie wszystkich swoich proces ́ow potomnych i wypisuje ich
identyfikatory PID oraz status zakończenia (zwracane przez wait), a w przypadku
zakończenia tych procesóww przez sygnał – również numer i nazwę odpowiedniego
sygnału. Pierwszy proces macierzysty również powinien zaczekać na zakończenie
swojego bezpośredniego potomka i wypisać jego PID oraz status zakończenia zwra-
cane przez odpowiednią funkcję wait.

Podpunkt (c): trzy programy - pierwszy tworzy jeden proces potomny i uruchamia
w nim program, który ustawia ignorowanie sygnału, staje się liderem swojej grupy
procesów, a następnie tworzy kilka procesów potomnych, które uruchamiają program
z podpunktu (a); pierwszy proces macierzysty, po krótkim czasie uśpienia
(funkcja sleep()) wysyła sygnał do całej grupy procesów; procesy macierzyste po
utworzeniu wszystkich swoich procesów potomnych powinny na nie poczekać
(funkcja wait() lub waitpid()), a następnie wypisać ich status zakończenia
i ewentualnie numer sygnału (w przypadku zakończenia procesu przez sygnał).

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
Program uruchamia w swoim procesie potomnym inny program pomocniczy,
który z kolei tworzy kilka procesów potomnych uruchamiających program
z zadania pierwszego.
Autor: Michał Śliż                   Kraków 12.04.2023
*/

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Błąd. Podano za mało argumentów wywołania programu. "
        "Podaj argumenty w postaci make run-cw_3c ARGS='nazwa_uruchamianego_programu numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    } else if (argc > 4) {
        printf("Błąd. Podano za dużo argumentów wywołania programu. "
        "Podaj argumenty w postaci make run-cw_3c ARGS='nazwa_uruchamianego_programu numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    }

    int fork_value;
    int group_pgid;
    int child_status; // status procesu potomnego
    int wait_status;
    int signal_num = strtoll(argv[2], NULL, 10);
    char path_name[N]; // nazwa uruchamianego programu
    sprintf(path_name, "%s%s", PATH, argv[1]);

    fork_value = fork();

    if (fork_value == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);

    // akcja dla procesu potomnego
    } else if (fork_value == 0) {
        // ustawienie lidera grupy procesów
        if (setpgid(0, 0) == -1) {
            perror("setpgid error");
            exit(EXIT_FAILURE);
        }

        // uruchomienie dodatkowego programu
        if (execlp(path_name, path_name, "cw_3a.x", argv[2], argv[3], NULL) == -1) {
            perror("execl error");
			exit(EXIT_FAILURE);
        }
    }
    // akcja dla procesu macierzystego
    else if (fork_value > 0) {
        sleep(2);
        group_pgid = getpgid(fork_value);

        // sprawdzenie czy grupa procesów intnieje
        if (kill(-group_pgid, 0) == -1) {
            perror("kill error");
            exit(EXIT_FAILURE);
        }
        // wysłanie sygnału do grupy procesów
        if (kill(-group_pgid, signal_num) == -1) {
            perror("kill error");
            exit(EXIT_FAILURE);
        }    
        
        wait_status = wait(&child_status);
        if(wait_status == -1)
        {
            perror("Wait error!\n");
            exit(EXIT_FAILURE);
        }

        printf("PID: %d, status zakończenia: %d\n", wait_status, child_status);
    }

    exit(EXIT_SUCCESS);
}
