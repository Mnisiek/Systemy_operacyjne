
#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define N_MAX 10 // maksymalna liczba wątków
int licznik_globalny = 0;
int n_watkow, n_petli;
pthread_mutex_t myMutex;
pthread_t tab_watkow[N_MAX]; // globalna tablica wątków o ustalonym rozmiarze

// funkcja ustawiająca kursor na odpowiednim miejscu
void set_cursor(unsigned x, unsigned y)
{
    printf("\033[%d;%dH\033[2K", y, x);
    fflush(stdout);
}

// funkcja uruchamiana w wątkach
void* thread_function(void* numer_watku_void)
{
    int j;
    int numer_watku = *((int *) numer_watku_void);
    printf("Numer wątku: %d, ID: %ld \n", numer_watku + 1, tab_watkow[numer_watku]);

    for (j = 0; j < n_petli; j++)
    {
        // sekcja prywatna
        sleep(1 + rand() % 3);
        set_cursor(0, 4 + n_watkow + numer_watku);
        printf("\033[34mNumer wątku: %d, obieg pętli: %d, wartość licznika globalnego: %d \033[0m", numer_watku + 1, j + 1, licznik_globalny);

            // sekcja krytyczna
        if (pthread_mutex_lock(&myMutex) != 0)
        {
            printf("Błąd przy zamknięciu muteksu");
            exit(EXIT_FAILURE);
        }

        // wyświetlenie danych
        set_cursor(70, 4 + numer_watku);
        set_cursor(0, 4 + n_watkow + numer_watku);
        printf("\033[34mNumer wątku: %d, obieg pętli: %d, wartość licznika globalnego: %d \033[0m", numer_watku + 1, j + 1, licznik_globalny);

        // zwiększenie wartości licznika
        int licznik;
        licznik = licznik_globalny;
        licznik++;
        sleep(1 + rand() % 3);
        licznik_globalny = licznik;

        if (pthread_mutex_unlock(&myMutex) != 0)
        {
            printf("Błąd przy otwarciu muteksu");
            exit(EXIT_FAILURE);
        }
            // koniec sekcji krytycznej
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Błąd, podano niewłaściwą liczbę argumentów");
        exit(EXIT_FAILURE);
    }

    int i;
    n_watkow = strtol(argv[1], NULL, 10);
    n_petli = strtol(argv[2], NULL, 10);

    if ((n_watkow == 0) || (n_petli == 0))
    {
        printf("Błąd, podaj właściwe wartości argumentów");
        exit(EXIT_FAILURE);
    }

    if (n_watkow > N_MAX)
    {
        printf("Błąd, przekroczono maksymalną liczbę wątków");
        exit(EXIT_FAILURE);
    }

    // tworzenie mutexu zainicjowanego na 0
    if (pthread_mutex_init(&myMutex, NULL) != 0)
    {
        printf("Błąd przy tworzeniu muteksu");
        exit(EXIT_FAILURE);
    }

    printf("Adres utworzonego muteksu: %p\n", (void*)&myMutex);

    // tworzenie wątków
    for (i = 0; i < n_watkow; ++i)
    {
        // rzutowanie int na void i przekazywanie wskaźnika do wątku
        int* memory = malloc(sizeof(int));
        *memory = i;
        if ((pthread_create(&tab_watkow[i], NULL, thread_function, (void *)memory)) != 0)
        {
            printf("Błąd przy tworzeniu wątków\n");
        };
        printf("Utworzono wątek o numerze %d, którego adres to %p\n", i + 1, 
        (void*)&tab_watkow[i]);
    }

    // czekanie na zakończenie wątków
    for (i = 0; i < n_watkow; ++i)
    {
        if (pthread_join(tab_watkow[i], NULL) != 0)
        {
            printf("Błąd podczas czekania na wątek");
            exit(EXIT_FAILURE);
        }
    }

    // sprawdzenie wyniku
    if (licznik_globalny == n_watkow * n_petli)
    {
        printf("\nSynchronizacja zakończyła się sukcesem:\n");
        printf("Końcowa wartość licznika globalnego: %d\n", licznik_globalny);
    }
    else {
        printf("\nSynchronizacja nie powiodła się\n");
        printf("Końcowa wartość licznika globalnego: %d\n", licznik_globalny);
    }

    if (pthread_mutex_destroy(&myMutex) != 0)
    {
        printf("Błąd przy zamykaniu muteksu");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
