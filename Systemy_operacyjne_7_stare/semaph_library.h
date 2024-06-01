
sem_t* semaphore_open(const char* name, int oflag);
sem_t* semaphore_create(const char* name, int oflag, mode_t mode, unsigned int value);
int semaphore_getvalue(sem_t* sem);
int semaphore_post(sem_t* sem);
int semaphore_wait(sem_t* sem);
int semaphore_close(sem_t* sem);
int semaphore_unlink(const char* name);
