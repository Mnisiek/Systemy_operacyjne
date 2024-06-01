#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CHUNK 16

int main() {

    char buffer[CHUNK], buffer2[CHUNK];
    ssize_t bytes_read, bytes_written;
    ssize_t pipe_bytes_read, pipe_bytes_written;

    int despipe[2];

    mkfifo("./potok.FIFO", 0644);
    despipe[0] = open("./potok.FIFO", O_RDONLY | O_NONBLOCK);
    despipe[1] = open("./potok.FIFO", O_WRONLY | O_NONBLOCK);

    printf ("Otwarto potok nienazwany z deskryptorami: %d %d\n", despipe[0], despipe[1]);

    ssize_t source = open("./test.txt", O_RDONLY);
    bytes_read = read(source, buffer, CHUNK);

    printf ("Odczytano %zd bajtow z pliku.\n", bytes_read);


    sleep(1);

    pipe_bytes_written = write(despipe[1], buffer, CHUNK);

    printf ("Przeslano %zd bajtow do potoku.\n", pipe_bytes_written);


    pipe_bytes_read = read(despipe[0], buffer2, CHUNK);
    
    printf ("Odczytano %zd bajtow z potoku.\n", pipe_bytes_read);

    printf ("Odczytane dane:\n%s\n", buffer2);

    ssize_t dest = open("./test_copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    bytes_written = write(dest, buffer2, CHUNK);
    
    printf ("Zapisano %zd bajtow.\n", bytes_written);

    unlink("./potok.FIFO");
    close(source);
    close(dest);
    exit (EXIT_SUCCESS);

}
