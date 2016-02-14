#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>

#define SHM_SIZE 1000000

void* allocMemory(size_t size){

void *ptr = nullptr;

key_t key = ftok("/tmp/mem.temp", 1);

auto id = shmget(key, SHM_SIZE,  IPC_CREAT);
if (id == -1){
	  perror("error shmget");
	  return nullptr;
}

//shmctl(id, IPC_RMID, NULL);
//shmid_ds datat;

//datat.shm_segsz = size;

//auto ret = shmctl(id, IPC_SET, &datat);

ptr = shmat(id, ptr, SHM_RND);
return ptr;

}
int main(int argc, char **argv){

enum {memsiz = 1000000};
char *ptr = reinterpret_cast<char *>(allocMemory(memsiz));
if (ptr){
	std::cout << "pointer is created" << std::endl;
	memset(ptr, 42, memsiz);
	//std::cout << ptr << std::endl;
}
else
	std::cout << "nullptr" << std::endl;
 
return 0;
}
