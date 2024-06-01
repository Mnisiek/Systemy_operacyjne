
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#ifndef SEMAFORY_BIBLIOTEKA
#include "semafory_biblioteka.h"
#define SEMAFORY_BIBLIOTEKA 0
#endif

#ifndef PAMIEC_DZIELONA_BIBLIOTEKA
#include "pamiec_dzielona_biblioteka.h"
#define PAMIEC_DZIELONA_BIBLIOTEKA 0
#endif

#ifndef CW_7_H
#include "cw_7.h"
#define CW_7_H 0
#endif

// deklaracje zmiennych globalnych
const char* sh_mem_name;
int sh_mem_des;
sem_t* sem_address_prod;
sem_t* sem_address_kons;
const char* sem_name_prod;
const char* sem_name_kons;

// funkcja zarejestrowana poprzez atexit()
void cleanup(void)
{
    printf("Usuwanie obiektu pamięci dzielonej i semafora\n");
    if ((sh_mem_name != NULL)) {
        sh_mem_unlink(sh_mem_name);
        sh_mem_close(sh_mem_des);
    }

    // sh_mem_unlink(sh_mem_name);
    // sh_mem_close(sh_mem_des);

    if ((sem_address_prod != NULL) && (sem_name_prod != NULL)) {
        semaphore_close(sem_address_prod);
        semaphore_unlink(sem_name_prod);
    }

    // semaphore_close(sem_address_prod);
    // semaphore_unlink(sem_name_prod);

    if ((sem_address_kons != NULL) && (sem_name_kons != NULL)) {
        semaphore_close(sem_address_kons);
        semaphore_unlink(sem_name_kons);
    }

    // semaphore_close(sem_address_kons);
    // semaphore_unlink(sem_name_kons);
}

// funkcja do obsługi sygnału SIGINT
void sigint_handler()
{
    printf("Otrzymano sygnał SIGINT\n");
    // cleanup();
    // sem_address_prod = NULL;
    // sem_address_kons = NULL;
    // sem_name_prod = NULL;
    // sem_name_kons = NULL;
    exit(EXIT_SUCCESS);
}

/*
Problem Producenta i Konsumenta z wykorzystaniem pamięci dzielonej
i semaforów standardu POSIX. Program korzysta z bufora cyklicznego,
umieszczonego w pamięci dzielonej, do której dostęp jest synchronizowany
semaforami.
*/

int main(int argc, char* argv[])
{
    // nazwa_programu_producenta, nazwa_programu_konsumenta,
    // nazwa_obiektu_pamięci_dzielonej,
    // nazwa_semafora_producenta, nazwa_semafora_konsumenta,
    // nazwa_pliku_wejściowego, nazwa_pliku_wyjściowego
    if (argc != 8) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    int fork_value1;
    int fork_value2;
    srand(time(NULL)); // inicjalizacja generatora liczb pseudolosowych

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

    // utworzenie obiektu pamięci dzielonej
    sh_mem_name = argv[3];
    sh_mem_des = sh_mem_create(sh_mem_name, O_RDWR, 0644, sizeof(SegmentPD));
    printf("Deskryptor pamięci dzielonej: %d, rozmiar pamięci dzielonej %d bajtów\n", sh_mem_des, (int) sizeof(SegmentPD));

    // utworzenie semafora do wstrzymywania Producenta
    sem_name_prod = argv[4];
    sem_address_prod = semaphore_create(sem_name_prod, O_CREAT | O_EXCL, 0644, N_BUF);
    printf("Adres semafora producenta: %p, wartość semafora: %d\n", (void*)sem_address_prod, semaphore_getvalue(sem_address_prod));

    // utworzenie semafora do wstrzymywania Konsumenta
    sem_name_kons = argv[5];
    sem_address_kons = semaphore_create(sem_name_kons, O_CREAT | O_EXCL, 0644, 0);
    printf("Adres semafora konsumenta: %p, wartość semafora: %d\n", (void*)sem_address_kons, semaphore_getvalue(sem_address_kons));

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
            if ((execlp(argv[1], argv[1], sh_mem_name, sem_name_prod, sem_name_kons, argv[6], (char*) NULL)) == -1) {
                perror("execlp error");
                exit(EXIT_FAILURE);
            }
        }
        // proces potomny wykonuje zadanie Konsumenta
        else {
            if ((execlp(argv[2], argv[2], sh_mem_name, sem_name_prod, sem_name_kons, argv[7], (char*) NULL)) == -1) {
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
