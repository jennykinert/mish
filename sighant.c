#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "mish.h"

/**
 * Name:signalCommand 
 * Description: If crt+c is pressed all children processes are killed 
 * @param signalCommand the SIGINT id
 * */
void signalCommand(int signalCommand) {
	if (signalCommand == SIGINT) {
		printf("recieved SIGINT\n");
		pid_t pid = getpid();
		kill(pid, SIGKILL);
	}
}

