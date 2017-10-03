#include <signal.h>
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
        int i =0;
        while(childID[i] != 0) {
            kill(childID[i],SIGKILL);
            i++;
        }
	}
}

