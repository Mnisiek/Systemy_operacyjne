
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg_queue_library.h"

mqd_t msq_open_readonly(const char *name)
{
    mqd_t desc;
    desc = mq_open(name, O_RDONLY);
    if (desc == -1)
    {
        perror("mq_open_readonly error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
    return desc;
}

mqd_t msq_open_writeonly(const char *name)
{
    mqd_t desc;
    desc = mq_open(name, O_WRONLY);
    if (desc == -1)
    {
        perror("mq_open_writeonly error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
    return desc;
}

mqd_t msq_create(const char *name, struct mq_attr *newattr)
{
    mqd_t desc;
    desc = mq_open(name, O_CREAT | O_EXCL | O_RDWR, 0666, newattr);
    if (desc == -1)
    {
        perror("mq_open_creating error");
        printf("%s\n", name);
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
    return desc;
}

void msq_close(mqd_t desc)
{
    if (mq_close(desc) == -1)
    {
        perror("mq_close error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void msq_unlink(const char *name)
{
    if (mq_unlink(name) == -1)
    {
        perror("mq_unlink error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void msq_send(mqd_t desc, const char *msg, int buffsize, int prio)
{
    int val;
    val = mq_send(desc, msg, buffsize, prio);
    if (val == -1)
    {
        perror("mq_send error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void msq_receive(mqd_t desc, char *buff, int buffsize)
{
    int val;
    val = mq_receive(desc, buff, buffsize, NULL);
    if (val == -1)
    {
        perror("mq_recieve error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void msq_getattr(mqd_t desc, struct mq_attr *attr)
{
    int val;
    val = mq_getattr(desc, attr);
    if(val ==-1)
    {
        perror("mq_getattr error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void msq_setattr(mqd_t desc, struct mq_attr *newattr, struct mq_attr *oldattr)
{
    int val;
    val = mq_setattr(desc, newattr, oldattr);
    if (val == -1)
    {
        perror("mq_setattr error");
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
    }
}