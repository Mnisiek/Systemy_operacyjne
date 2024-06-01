/*
Napisać program do obsługi sygnałów z możliwościami: (1) wykonania operacji domy-
ślnej, (2) ignorowania oraz (3) przechwycenia i własnej obsługi sygnału. Do oczeki-
wania na sygnał użyć funkcji pause. Uruchomić program i wysyłać do niego sygnały
przy pomocy sekwencji klawiszy oraz przy pomocy polecenia kill z poziomu powłoki.

Podpunkt (a): jeden program - ustawia obsługę sygnału na 3 sposoby zgodnie z opcją
podaną jako argument wywołania programu, a następnie czeka na sygnał
(funkcja pause()); numer sygnału przekazywać jako argument wywołania programu;
niech proces wypisze na ekranie swój PID, a funkcja do własnej obsługi sygnału
jego numer i nazwę (można wstawić krótki sleep).

Numer sygnału oraz opcje jego obsługi we wszystkich powyższych programach przeka-
zywać za pomocą argumentów wywołania programu – sprawdzać ich liczbę i wypisywać
odpowiedni komunikat w przypadku błędnego uruchomienia (ze wskazówką poprawnego
uruchomienia).
*/

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#include <unistd.h> //pause()
#include <stdio.h>  //printf() & perror()
#include <signal.h> //signal()
#include <string.h> //strcmp() & strsignal()
#include <stdlib.h> //exit()

void signal_handler(int signal_num); // funkcja do obsługi sygnału


/*
Program ustawia obsługę wybranego sygnału na trzy sposoby: domyślny, ignorowanie, wywołanie funkcji obsługi sygnału.
Autor: Michał Śliż                   Kraków 12.04.2023
*/

int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("Błąd. Podano za mało argumentów wywołania programu. "
        "Podaj argumenty w postaci make run-cw_3a ARGS='numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    } else if (argc > 3) {
        printf("Błąd. Podano za dużo argumentów wywołania programu. "
        "Podaj argumenty w postaci make run-cw_3a ARGS='numer_sygnału opcja_obsługi_sygnału'\n");
        exit(EXIT_FAILURE);
    }

    int signal_num = strtol(argv[1], NULL, 10);

    // niepoprawny numer sygnału
    if (signal_num < 1 || signal_num > 64) {
        printf("Błąd. Podano nieprawidłowy numer sygnału. Lista sygnałów i odpowiadających im numerów:\n");
        printf(" 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP\n"
        "6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1\n"
        "11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM\n"
        "16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP\n"
        "21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ\n"
        "26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR\n"
        "31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3\n"
        "38) SIGRTMIN+4  39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8\n"
        "43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13\n"
        "48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12\n"
        "53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7\n"
        "58) SIGRTMAX-6  59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2\n"
        "63) SIGRTMAX-1  64) SIGRTMAX\n");
        exit(EXIT_FAILURE);
    }

    if (!(strcmp(argv[2], "default"))) {
        signal(signal_num, SIG_DFL); // domyślna obsługa sygnału

    } else if (!(strcmp(argv[2], "ignore"))) {
        signal(signal_num, SIG_IGN); // ignorowanie sygnału

    } else if (!(strcmp(argv[2], "intercept"))) {
        signal(signal_num, signal_handler); // przechwycenie sygnału

    } else {
        printf("Błąd. Podano niewłaściwą opcję obsługi sygnału. Dostępne opcje to: 'default', 'ignore', 'intercept'\n");
        exit(EXIT_FAILURE);
    }

    printf("PID procesu: %d\n", getpid());

    pause(); // program oczekuje na sygnał
    exit(EXIT_SUCCESS);
}


// prosta funkcja do obsługi przechwyconego sygnału
void signal_handler(int signal_num)
{
    printf("\nNumer przechwyconego sygnału: %d\n", signal_num);
    printf ("Sygnał %s został przechwycony\n", strsignal(signal_num));
    sleep(2);
}
