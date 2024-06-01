
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#ifndef BIBLIOTEKA
#include "biblioteka.h"
#define BIBLIOTEKA 0
#endif

// deklaracje zmiennych globalnych
sem_t* semaphore_adress;
const char* semaphore_name;

// funkcja zarejestrowana poprzez atexit()
void cleanup(void)
{
    printf("Usuwanie semafora...\n");
    if ((semaphore_adress != NULL) && (semaphore_name != NULL)) {
        semaphore_close(semaphore_adress);
        semaphore_unlink(semaphore_name);
    }
}

// funkcja do obsługi sygnału SIGINT
void sigint_handler()
{
    printf("Otrzymano sygnał SIGINT.\n");
    // cleanup();
    // semaphore_adress = NULL;
    // semaphore_name = NULL;
    exit(EXIT_SUCCESS);
}

/*
Program powiela procesy realizujące wzajemne wykluczanie. Tworzy procesy
potomne korzystając z funkcji fork(), a następnie z wykorzystaniem funkcji
execlp() uruchamia w nich kod programu cw_6_wykluczanie.c.
*/

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    // rejestrowanie funkcji cleanup() poprzez atexit()
    if (atexit(cleanup) != 0) {
        perror("atexit error");
        exit(EXIT_FAILURE);
    }

    // ustawianie obsługi sygnału Ctrl-C
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal error");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL)); // inicjalizacja generatora liczb pseudolosowych

    int file_des;
    int init_value;
    int final_value;
    int fork_value;
    int child_status;

    if ((file_des = open(argv[5], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("create and open file error");
        exit(EXIT_FAILURE);
    }

    init_value = 0;
    if (write(file_des, &init_value, sizeof(int)) == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }

    // utworzenie semafora o podanej nazwie i zainicjalizowanie go wartością 1
    semaphore_name = argv[4];
    semaphore_adress =  semaphore_create(semaphore_name, O_CREAT | O_EXCL, 0644, 1);

    printf("Adres semafora: %p, wartość początkowa: %d\n", 
    (void*) semaphore_adress, semaphore_getvalue(semaphore_adress));

    for (int i = 0; i < strtol(argv[2], NULL, 10); ++i) {

        fork_value = fork();
        if (fork_value == -1) {
            perror("fork error");
            exit(EXIT_FAILURE);

        // proces potomny
        } else if (fork_value == 0) {
            if (execlp(argv[1], argv[1], argv[3], argv[4], argv[5], (char*) NULL) == -1) {
                perror("execlp error");
                exit(EXIT_FAILURE);
            }
        }

        // proces macierzysty
        else {
            // printf("Status procesu potomnego o PID %d: %d\n", fork_value, child_status);

        //     if ((file_des = open(argv[5], O_RDONLY)) == -1) {
        //         perror("open file error");
        //         exit(EXIT_FAILURE);
        //     }
        //     if (read(file_des, &final_value, sizeof(int)) == -1) {
        //         perror("read error");
        //         exit(EXIT_FAILURE);
        //     }

        // }
    }}

    for (int i = 0; i < strtol(argv[2], NULL, 10); ++i) {
        wait(&child_status);
    }
    
    if ((file_des = open(argv[5], O_RDONLY)) == -1) {
                perror("open file error");
                exit(EXIT_FAILURE);
            }
    if (read(file_des, &final_value, sizeof(int)) == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
    }

    printf("Liczba odczytana z pliku po zakończeniu synchronizacji procesów: %d\n", final_value);
    printf("Liczba wywołanych procesów: %ld, liczba sekcji krytycznych każdego procesu: %ld\n", strtol(argv[2], NULL, 10), strtol(argv[3], NULL, 10));

    if (strtol(argv[2], NULL, 10) * strtol(argv[3], NULL, 10) == final_value) {
        printf("Program wykonał się poprawnie\n");
    }

    exit(EXIT_SUCCESS);

    }
