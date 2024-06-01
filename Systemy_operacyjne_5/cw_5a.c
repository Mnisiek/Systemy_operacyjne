/*
Implementacja problemu "Producent-konsument" jak w ćwiczeniu 4.
Najlepiej zaimplementować "producenta" i "konsumenta" w dwóch oddzielnych 
programach (funkcjach main()) - każdy z nich niech przez argumenty wywołania 
pobiera nazwę potoku oraz nazwę odpowiedniego pliku tekstowego;
potok nazwany - tak jak plik - przed użyciem należy otworzyć funkcją open(),
a po użyciu zamknąć funkcją close().
W punkcie (a) napisać dodatkowy program, który utworzy potok nazwany,
następnie przy pomocy funkcji fork() utworzy dwa procesy potomne, w których
przy użyciu wywołania exec uruchomi odpowiednio programy "producenta"
i "konsumenta", a na końcu poczeka na ich zakończenie i usunie potok
(nazwy plików wykonawczych dla exec przekazywać przez argumenty wywołania
programu).
Potok usuwać w funkcji zarejestrowanej przez funkcję atexit().
Zwrócić uwagę na to, gdzie należy wywołać funkcję exit(),
a gdzie _exit() - by nie usuwać potoku, który nie został jeszcze utworzony
lub nie usuwać go więcej niż raz! Dodatkowo, można dodać obsługę sygnału
SIGINT, która będzie usuwać potok - na wypadek gdyby program trzeba było
zakończyć sekwencją klawiszy Ctrl-C.
Nazwy plików danych i wyników oraz nazwę potoku przekazywać przez argumenty
wywołania programu.
W punkcie (b) potok nazwany utworzyć z poziomu powłoki, następnie uruchomić
programy "producenta" i "konsumenta" z różnych terminali, a na końcu z poziomu
powłoki usunąć potok. Najlepiej wszystkie te polecenia umieścić w pliku
Makefile, np. programy uruchomić za pomocą xterm:

xterm -hold -title PRODUCENT -e ./producent.x mojfifo dane.txt &

xterm -hold -title KONSUMENT -e ./konsument.x mojfifo wyniki.txt

Programy "producent" i "konsument" niech wypisują na ekranie komunikaty o
przesyłanych danych, jak w ćwiczeniu 4; na końcu sprawdzić poprawność wykonania
programów przez porównanie pliku danych "producenta" z plikiem wyników
"konsumenta", np. przy pomocy polecenia diff -s
UWAGA: Fukcją  write() wpisywać liczbę faktycznie wczytanych przez read()
bajtów (zwracaną prze nią).
Programy kompilować/linkować z flagami: -Wall -std=c99 -pedantic
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

void cleanup()
{
    unlink("mojfifo");
}

/*
Problem Producenta i Konsumenta z wykorzystaniem potoków nazwanych (FIFO).
Proces potomny i potomny procesu potomnego komunikują się między sobą, a
proces macierzysty czeka na ich zakończenie.*/

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n"
        "Uruchom program wywołując polecenie make run-cw_5a. "
        "Możesz zmienić domyślne argumenty w pliku Makefile lub nadpisując je w wywołaniu.\n");
        exit(EXIT_FAILURE);
    }

    int fork_value1;
    int fork_value2;
    srand(time(NULL)); // inicjalizacja generatora liczb pseudolosowych

    atexit(cleanup);

    if (mkfifo(argv[3], 0644) == -1) {
        if (errno != EEXIST) {
            perror("create FIFO error");
            exit(EXIT_FAILURE);
        }
    }

    fork_value1 = fork();
    if (fork_value1 == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // proces potomny
    else if (fork_value1 == 0) {
        fork_value2 = fork();
        if (fork_value2 == -1) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        // proces potomny procesu potomnego wykonuje zadanie Producenta
        else if (fork_value2 == 0) {
            if (execlp(argv[4], argv[4], argv[3], argv[1], (char*) NULL) == -1) {
                perror("execlp error");
                exit(EXIT_FAILURE);
            }
        }
        // proces potomny wykonuje zadanie Konsumenta
        else {
            if (execlp(argv[5], argv[5], argv[3], argv[2], (char*) NULL) == -1) {
                perror("execlp error");
                exit(EXIT_FAILURE);
            }
        }
    }

    // proces macierzysty
    else {
        // proces macierzysty czeka na zakończenie swojego procesu potomnego
        int child_status;
        if (wait(&child_status) == -1) {
            perror("wait error");
            exit(EXIT_FAILURE);
            }
    }

    exit(EXIT_SUCCESS);
}