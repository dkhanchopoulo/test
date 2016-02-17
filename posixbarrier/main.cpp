#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <iostream>
#include <fstream>

#define pidfile "main.pid"
//#define pidfile "/home/box/main.pid"

pthread_t tid[4];
pthread_mutex_t lock;
pthread_cond_t      condit  = PTHREAD_COND_INITIALIZER;
bool doneCondit = false;

pthread_barrier_t   barrier; // barrier synchronization object

///////////////////////////////////
void * waitConditThread(void *arg){
pthread_mutex_lock(&lock);
std::cout << __FUNCTION__ << " mxlock" << std::endl;
while(!doneCondit)
	pthread_cond_wait(&condit,&lock);
std::cout << __FUNCTION__ << "condition appear!" << std::endl;
pthread_mutex_unlock(&lock);
return NULL;
}

///////////////////////////////////////////////
void *waitBarrierThread(void *arg){

std::cout << __FUNCTION__ << " : " << "wait" << std::endl; 
pthread_barrier_wait (&barrier);
std::cout << __FUNCTION__ << "done" << std::endl;

return NULL;
}


void markPID()
{
	std::ofstream f (pidfile);

	f << getpid() << std::endl;
}
int main(int argc, char **argv){

markPID();

//locking
pthread_barrier_init (&barrier, NULL, 2);
//
//initthreads
auto err = pthread_create(&(tid[0]), NULL, &waitConditThread, NULL);
if (err !=0)
	std::cout << "can't create thread : [" << strerror(err) << "] " << std::endl; 

err = pthread_create(&(tid[1]), NULL, &waitBarrierThread, NULL);
if (err !=0)
	std::cout << "can't create thread : [" << strerror(err) << "] " << std::endl; 

//
std::cout << "sleeping" << std::endl;
sleep(3);
pthread_mutex_lock(&lock);
doneCondit = true;
pthread_cond_signal(&condit);
pthread_mutex_unlock(&lock);

//

pthread_barrier_wait (&barrier);
 
pthread_join(tid[0], NULL);
pthread_join(tid[1], NULL);

//deinit all
pthread_mutex_destroy(&lock);
std::cout << "finished" << std::endl;


return 0;
}
