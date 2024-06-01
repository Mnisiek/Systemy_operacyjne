
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "msg_queue_library.h"

mqd_t serverDesc, clientDesc;
char cName[10];

/*tworzenie funkcji rejestrowanych przez atexit i signal*/

void sQueueClose()
{
    printf("server queue closing...\n");
    msq_close(serverDesc);
}

void cQueueClose()
{
    printf("client queue closing...\n");
    msq_close(clientDesc);
    msq_unlink(cName);
}

void exit_signal(int signal)
{
    printf("\nExit signal, queue closing\n");
    printf("Server process exiting...\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    /*obsługa sygnału*/
    if (signal(SIGINT, exit_signal) == SIG_ERR)
    {
        perror("Signal error:");
        exit(EXIT_FAILURE);
    }

    atexit(sQueueClose);
    atexit(cQueueClose);

    /*definiowanie zmiennych*/
    int pid;
    struct mq_attr atributes;
    char msgReceived[60];
    char str[70];
    char strRead[60];

    /*odczytywanie pid procesu kilenta*/
    pid = getpid();
    sprintf(cName, "/%d", pid);

    /*ustawianie atrybutów*/
    struct mq_attr newattr;
    newattr.mq_flags = 0;
    newattr.mq_maxmsg = 10;
    newattr.mq_msgsize = MSGLENGTH;
    newattr.mq_curmsgs = 0;

    /*otwieranie kolejek dla klienta i serwera*/
    clientDesc = msq_create(cName, &newattr);
    clientDesc = msq_open_readonly(cName);
    serverDesc = msq_open_writeonly(SERVERQUEUE);

    /*wyświetlanie atrybutów na konsoli*/
    msq_getattr(clientDesc, &atributes);
    printf("Created a queue \"%s\" which has descriptor \"%d\" and atributes:\n", SERVERQUEUE, clientDesc);
    printf("mq_flags: = %ld\n", atributes.mq_flags);
    printf("mq_maxmsg: = %ld\n", atributes.mq_maxmsg);
    printf("mq_msgsize: = %ld\n", atributes.mq_msgsize);
    printf("mq_curmsgs: = %ld\n\n", atributes.mq_curmsgs);

    printf("Waiting for new message\n");
    /*wysyłanie komunikatów do serwera*/
    while (fgets(strRead, sizeof(strRead), stdin) != NULL)
    {
        sprintf(str, "%d %s", pid, strRead);
        printf("%s\n", str);
        sleep(rand() % 10);
        msq_send(serverDesc, str, MSGLENGTH, 0);
        printf("Message sent to server\n");
        /*odczytanie komunikatu z serwera*/
        msq_receive(clientDesc, msgReceived, MSGLENGTH);
        /*wypisanie komunikatu na konsolę*/
        printf("Answer received: %s\n", msgReceived);
        printf("\nWaiting for new message\n");
    }

    printf("Client process exiting...\n");
    exit(EXIT_SUCCESS);
}