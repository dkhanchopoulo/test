#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef localdbg
	#define fifoIn "/home/box/in.fifo"
	#define fifoOut "/home/box/out.fifo"
#else
	#define fifoIn "./in.fifo"
	#define fifoOut "./out.fifo"
#endif
int main(int argc, char **argv){

	mkfifo(fifoIn, 0666);
	mkfifo(fifoOut,0666);

	printf("Welcome to fifo server\n");

	int inf = open(fifoIn, O_RDONLY);
	if (inf == -1){
		perror("open error");
		exit(1);
	}

	auto off = open(fifoOut, O_WRONLY);
	if (inf == -1){
		perror("error write file");
		exit(1);
	}

	auto len = -1;

	enum { buffsize  = 512};

	char buffer[buffsize] = {0};
	while((len = read(inf, buffer, buffsize)) > 0)
	{
		printf("Received: %s\n", buffer);
		write(off, buffer, len); 
	}


return 0;
}

