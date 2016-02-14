
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>


#define SHM_SIZE 1048576

void* allocMemory(size_t size){

void *ptr = nullptr;

auto key = "/test.shm";

int shm_fd;
shm_fd = shm_open(key, O_CREAT | O_RDWR, 0644);
ftruncate(shm_fd, SHM_SIZE);

ptr = mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
if (ptr == MAP_FAILED) {
	std::cout << "mapping failed..." << std::endl;
	return nullptr;
} 

return ptr;

}
int main(int argc, char **argv){

enum {memsiz = 1048576};
char *ptr = reinterpret_cast<char *>(allocMemory(memsiz));
if (ptr){
	std::cout << "pointer is created" << std::endl;
	memset(ptr, 42, memsiz);
	//std::cout << ptr << std::endl;
}
else
	std::cout << "nullptr" << std::endl;
 
std::cin.ignore().get();
return 0;
}
