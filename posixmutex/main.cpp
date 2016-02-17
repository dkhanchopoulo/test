#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <iostream>
#include <fstream>

#define pidfile "main.pid"
//#define pidfile "/home/box/main.pid"
pthread_mutex_t lock;
pthread_spinlock_t spinlock;

pthread_rwlock_t  rwlock = PTHREAD_RWLOCK_INITIALIZER;
	 
int pshared;

pthread_t tid[4];


void * waitMutexThread(void *arg){

pthread_mutex_lock(&lock);
std::cout << __FUNCTION__ << " mxlock" << std::endl;
pthread_mutex_unlock(&lock);
std::cout << __FUNCTION__ << "mxunlocked " << std::endl;
return NULL;
}

void *waitSpinThread(void *arg){

auto ret = pthread_spin_lock(&spinlock);
std::cout << __FUNCTION__ << " : " << "locked" << std::endl;
ret = pthread_spin_unlock(&spinlock); 
std::cout << __FUNCTION__ << "finished" << std::endl;

return NULL;
}

void * waitRWThread(void *args){

bool *read = reinterpret_cast<bool *>(args);
if (*read){
	pthread_rwlock_rdlock(&rwlock);
	std::cout << " **  read" << std::endl;
}
else
	{
	pthread_rwlock_wrlock(&rwlock);
	std::cout << " ** write" << std::endl;
}
pthread_rwlock_unlock(&rwlock);
return NULL;
}

void markPID()
{
	std::ofstream f (pidfile);

	f << getpid() << std::endl;
}
int main(int argc, char **argv){

bool bRead = true;
bool bReadWrite = false;

markPID();

int ret = pthread_spin_init(&spinlock, pshared);
ret = pthread_rwlock_init(&rwlock, NULL);

//locking
ret = pthread_spin_lock(&spinlock);
ret = pthread_mutex_lock(&lock);
ret = pthread_rwlock_wrlock(&rwlock);
//
//initthreads
auto err = pthread_create(&(tid[0]), NULL, &waitSpinThread, NULL);
if (err !=0)
	std::cout << "can't create thread : [" << strerror(err) << "] " << std::endl; 

err = pthread_create(&(tid[1]), NULL, &waitMutexThread, NULL);
if (err !=0)
	std::cout << "can't create thread : [" << strerror(err) << "] " << std::endl; 

err = pthread_create(&(tid[2]), NULL, &waitRWThread, &bRead);
if (err !=0)
	std::cout << "can't create thread : [" << strerror(err) << "] " << std::endl; 

err = pthread_create(&(tid[3]), NULL, &waitRWThread, (void*)&bReadWrite);
if (err !=0)
	std::cout << "can't create thread : [" << strerror(err) << "] " << std::endl; 




//
std::cout << "spin unlocking" << std::endl;
sleep(3);

ret = pthread_spin_unlock(&spinlock);
ret = pthread_mutex_unlock(&lock);
ret = pthread_rwlock_unlock(&rwlock);


// 
pthread_join(tid[0], NULL);
pthread_join(tid[1], NULL);
pthread_join(tid[2], NULL);
pthread_join(tid[3], NULL);

//deinit all
ret = pthread_spin_destroy(&spinlock);
ret = pthread_mutex_destroy(&lock);
ret = pthread_rwlock_destroy(&rwlock);

std::cout << "finished" << std::endl;


return 0;
}
