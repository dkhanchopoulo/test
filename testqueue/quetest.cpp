#include<string.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<sys/errno.h>
#include <fstream>

struct message {
    long mtype;
    char mtext[80];
};

const char outfile[]="~/message.txt";
#define MSGPERM 0600

int main(){


	key_t key = ftok("/tmp/msg.temp", 1);
	
	auto msgqid = msgget(key, MSGPERM |IPC_CREAT);

	if (msgqid < 0) {
  	  perror(strerror(errno));
   	 printf("failed to create message queue with msgqid = %d\n", msgqid);
    	return 1;
  	}
 	 printf("message queue %d created\n",msgqid);
	
	message msg;

	auto rc = msgrcv(msgqid, &msg, sizeof(message), 0, 0);
	if (rc < 0){
		perror("error msgrcv");
	return 1;	
	}

	std::ofstream f(outfile);
	f << msg.mtext;

	rc = msgctl(msgqid, IPC_RMID, NULL);
	
	return 0;

}
