/*
Przy pomocy potoków nienazwanych systemu UNIX zaimplementować problem "Pro-
ducenta i konsumenta". Dla zademonstrowania, że nie doszło do utraty ani 
zwielokrotnienia towaru, niech Producent pobiera "surowiec" (np. porcje bajtów) 
z pliku tekstowego i wstawia go jako towar do potoku, a Konsument niech 
umieszcza pobrany z potoku towar w innym pliku tekstowym (porcje danych 
Producenta i Konsumenta nie muszą być równe). 
Po zakończeniu działania programów (wyczerpaniu zasobów "surowca") oba pliki
tekstowe powinny być identyczne (można to sprawdzić poleceniem dif -s , które
najlepiej umieścić w pliku Makefile po poleceniu uruchomienia programu).
Oba procesy niech drukują odpowiednie komunikaty na ekranie, w tym towar, który 
przesyłają. Do zasymulowania różnych szybkości działania programów użyć funkcji 
sleep(), np. z losowym czasem usypiania. 
Do czytania/pisania danych z/do pliku tekstowego, jak również wypisywania
ich na ekran użyć funkcji read i write.

Dane do potoku przesyłać/pobierać małymi porcjami (po kilka bajtów, 
nie wszystko naraz!) i wypisywać na ekranie dane przesyłane za każdym razem 
producenta i konsumenta; w ogólności porcje danych producenta i konsumenta mogą 
być różne, dlatego powinny zostać zdefiniowane oddzielnie.
Nazwy plików danych i wyników przekazywać przez argumenty wywołania programu.
Do czytania/pisania danych z/do pliku użyć funkcji read() i write().
Do wypisywania danych na ekranie również użyć funkcji write() z odpowiednim 
deskryptorem.
Do symulowania różnych szybkości działania procesów użyć funkcji sleep()
z losowym czasem usypiania generowanym np. przez funkcję rand()(patrz: man 3 rand).
UWAGA: Fukcją write() wpisywać liczbę faktycznie wczytanych przez read() bajtów
(zwracaną prze nią).
Kompilacja/linkowanie z flagami: -Wall -std=c99 -pedantic 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define N_READ 4
#define N_WRITE 6
#define N_INFO 14


/*
Problem Producenta i Konsumenta. Producent odczytuje dane z pliku i przesyła je
przy pomocy potoku nienazwanego do swojego procesu macierzystego, który zapisuje
dane do innego pliku.*/

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n"
        "Uruchom program wywołując polecenie make run-cw_4. "
        "Możesz zmienić domyślne argumenty w pliku Makefile lub nadpisując je w wywołaniu.\n");
        exit(EXIT_FAILURE);
    }

    char buffer1[N_READ];
    char buffer2[N_WRITE];
    char buffer3[N_INFO];
    char* message;
    int length;
    int bytes_read, bytes_written;
    int source_des, destination_des; // deskryptory plików
    int fork_value;
    int pipe_des[2]; // tablica zawierająca deskryptory potoku
    srand(time(NULL)); // inicjalizacja generatora liczb pseudolosowych

    if (pipe(pipe_des) == -1) {
        perror("create pipe error");
        exit(EXIT_FAILURE);
    }

    fork_value = fork();
    if (fork_value == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // proces potomny
    else if (fork_value == 0) {
        // zamknięcie nieużywanego deskryptora potoku (do odczytu)
        if (close(pipe_des[0]) == -1) {
            perror("close pipe reading end error");
            exit(EXIT_FAILURE);
        }
        if ((source_des = open(argv[1], O_RDONLY)) == -1) {
            perror("open source file error");
            exit(EXIT_FAILURE);
        }
        // pętla odczytująca dane z pliku wejściowego
        while ((bytes_read = read(source_des, buffer1, N_READ)) != 0) {
            if (bytes_read == -1) {
                perror("reading file error");
                exit(EXIT_FAILURE);
            }
            // program czeka losową liczbę sekund (z przedziału 0 - 2)
            sleep(rand() % 3);
            message = "\nOdczytano z pliku: ";
            length = strlen(message);
            write(STDOUT_FILENO, message, length);
            write(STDOUT_FILENO, buffer1, bytes_read);
            sprintf(buffer3, " (%d bajtów)", bytes_read);
            write(STDOUT_FILENO, buffer3, N_INFO);

            if ((bytes_written = write(pipe_des[1], buffer1, bytes_read)) == -1) {
                perror("writting pipe error");
                exit(EXIT_FAILURE);
            }
            sleep(rand() % 3);
            message = "\nZapisano do potoku: ";
            length = strlen(message);
            write(STDOUT_FILENO, message, length);
            write(STDOUT_FILENO, buffer1, bytes_written);
        }

        message = "\nOczytano całą zawartość pliku wejściowego\n";
        length = strlen(message);
        write(STDOUT_FILENO, message, length);

        // zamknięcie otwartych deskryptorów pliku i potoku
        if (close(source_des) == -1) {
            perror("close source file error");
            exit(EXIT_FAILURE);
        }
        if (close(pipe_des[1]) == -1) {
            perror("close pipe writting end error");
            exit(EXIT_FAILURE);
        }
    }

    // proces macierzysty
    else {
        // zamknięcie nieużywanego deskryptora potoku (do zapisu)
        if (close(pipe_des[1]) == -1) {
            perror("close pipe writting end error");
            exit(EXIT_FAILURE);
        }
        // proces macierzysty czeka na zakończenie swojego procesu potomnego
        int child_status;
        if (wait(&child_status) == -1) {
            perror("wait error");
            exit(EXIT_FAILURE);
        }

        if ((destination_des = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            perror("open destination file error");
            exit(EXIT_FAILURE);
        }
        // pętla odczytująca dane z potoku
        while ((bytes_read = read(pipe_des[0], buffer2, N_WRITE)) != 0) {
            if (bytes_read == -1) {
                perror("reading pipe error");
                exit(EXIT_FAILURE);
            }
            // program czeka losową liczbę sekund (z przedziału 0 - 2)
            sleep(rand() % 3);
            message = "\nOdczytano z potoku: ";
            length = strlen(message);
            write(STDOUT_FILENO, message, length);
            write(STDOUT_FILENO, buffer2, bytes_read);

            if ((bytes_written = write(destination_des, buffer2, bytes_read)) == -1) {
                perror("writting file error");
                exit(EXIT_FAILURE);
            }
            sleep(rand() % 3);
            message = "\nZapisano do pliku: ";
            length = strlen(message);
            write(STDOUT_FILENO, message, length);
            write(STDOUT_FILENO, buffer2, bytes_written);
            sprintf(buffer3, " (%d bajtów)", bytes_written);
            write(STDOUT_FILENO, buffer3, N_INFO);
        }

        message = "\nZapisano całą zawartość potoku\n";
        length = strlen(message);
        write(STDOUT_FILENO, message, length);

        // zamknięcie otwartych deskryptorów pliku i potoku
        if (close(destination_des) == -1) {
            perror("close destination file error");
            exit(EXIT_FAILURE);
        }
        if (close(pipe_des[0]) == -1) {
            perror("close pipe reading end error");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

