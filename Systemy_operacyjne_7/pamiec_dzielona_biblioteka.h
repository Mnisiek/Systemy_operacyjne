
int sh_mem_open(const char* name, int opening, mode_t mode);
int sh_mem_create(const char* name, int opening, mode_t mode, unsigned int length);
int sh_mem_close(int sh_mem_des);
int sh_mem_unlink(const char* name);
void* sh_mem_mmap(void* address, size_t length, int prot, int flags, int sh_mem_des, off_t offset);
int sh_mem_munmap(void* address, size_t length);