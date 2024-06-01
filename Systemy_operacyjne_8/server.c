
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "msg_queue_library.h"

mqd_t serverDesc, clientDesc;

/*funkcje dla atexit i signal*/
void sQueueClose()
{
    printf("server queue closing...\n");
    msq_close(serverDesc);
    msq_unlink(SERVERQUEUE);
}

void cQueueClose()
{
    printf("client queue closing...\n");
    msq_close(clientDesc);
}

void exit_signal(int signal)
{
    printf("\nExit signal, queue closing\n");
    atexit(sQueueClose);
    atexit(cQueueClose);
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

    /*tworzenie zmiennych*/
    struct mq_attr atributes;
    char msgReceived[MSGLENGTH];
    char msgSent[MSGLENGTH];
    int pid, num1, num2, result;
    char operator;
    char cName[10];

    /*ustawianie atrybutów*/
    struct mq_attr newattr;
    newattr.mq_flags = 0;
    newattr.mq_maxmsg = 10;
    newattr.mq_msgsize = MSGLENGTH;
    newattr.mq_curmsgs = 0;

    /*otwieranie kolejki serwera*/
    serverDesc = msq_create(SERVERQUEUE, &newattr);
    serverDesc = msq_open_readonly(SERVERQUEUE);

    /*wypisywanie atrybutów kolejki na konsoli*/
    msq_getattr(serverDesc, &atributes);
    printf("Created a queue \"%s\" which has descriptor \"%d\" and atributes:\n", SERVERQUEUE, serverDesc);
    printf("mq_flags: = %ld\n", atributes.mq_flags);
    printf("mq_maxmsg: = %ld\n", atributes.mq_maxmsg);
    printf("mq_msgsize: = %ld\n", atributes.mq_msgsize);
    printf("mq_curmsgs: = %ld\n\n", atributes.mq_curmsgs);

    while (1)
    {
        /*odczytywanie wiadomości i pidu procesu klienta*/
        printf("Waiting for message...\n");
        msq_receive(serverDesc, msgReceived, MSGLENGTH);
        printf("Message received: %s\n", msgReceived);
        sleep(rand() % 10);
        sscanf(msgReceived, "%d %d%c%d", &pid, &num1, &operator, & num2);
        sprintf(cName, "/%d", pid);
        printf("%d %d %c %d\n", pid, num1, operator, num2);

        /*otwieranie kolejki procesu klienta*/
        printf("Client queue opening\n");
        clientDesc = msq_open_writeonly(cName);

        /*obliczanie wyniku*/
        if (operator== '+')
        {
            printf("calculating %d + %d\n", num1, num2);
            result = num1 + num2;
            sprintf(msgSent, "%d", result);
        }
        else if (operator== '-')
        {
            printf("calculating %d - %d\n", num1, num2);
            result = num1 - num2;
            sprintf(msgSent, "%d", result);
        }
        else if (operator== '*')
        {
            printf("calculating %d * %d\n", num1, num2);
            result = num1 * num2;
            sprintf(msgSent, "%d", result);
        }
        else if (operator== '/')
        {
            printf("calculating %d / %d\n", num1, num2);
            result = num1 / num2;
            sprintf(msgSent, "%d", result);
        }
        else
        {
            sprintf(msgSent, "Error: unknown operator\n");
        }

        /*wysyłanie wiadomości i zamykanie kolejki klienta*/
        printf("Sending answer to client queue\n");
        msq_send(clientDesc, msgSent, MSGLENGTH, 0);
        msq_close(clientDesc);
    }
}