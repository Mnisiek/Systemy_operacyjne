#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CHUNK 16

int main() {

    char buffer2[CHUNK];
    ssize_t bytes_written;
    ssize_t pipe_bytes_read;

    int despipe[2];

    mkfifo("./potok.FIFO", 0644);
    despipe[0] = open("./potok.FIFO", O_RDONLY);

    printf ("Otwarto potok nazwany z deskryptorem: %d\n", despipe[0]);


    pipe_bytes_read = read(despipe[0], buffer2, CHUNK);
    
    printf ("Odczytano %zd bajtow z potoku.\n", pipe_bytes_read);

    printf ("Odczytane dane:\n%s\n", buffer2);

    ssize_t dest = open("./test_copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    bytes_written = write(dest, buffer2, pipe_bytes_read);
    
    printf ("Zapisano %zd bajtow.\n", bytes_written);

    unlink("./potok.FIFO");
    close(dest);
    exit (EXIT_SUCCESS);

}
