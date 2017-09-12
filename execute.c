#include <stdio.h>
#include <unistd.h>
#include "execute.h"

int dubPipe(int pip[2], int end, int destfd)
{
	//Creates a pair of file descriptors pointing to pipe inode pip[0]=reading pip[1]=writing
	pipe(pip); 
	if(end == 0){
		dup(pip[0]);
		close(pip[1]);
		printf("%d \n",destfd);
		return destfd; 
	}
	else if(end == 1){
		dup(pip[1]); 
		close(pip[0]); 
		return destfd; 
	}
	else {
		perror("pipe"); 
		return -1; 
	}
}

int redirect(char *filename, int flags, int destfd){
	return 0; 
}

int main(){
	printf("Jag gick in i main \n"); 
	int fd[2]; 
	int end = 0;
	printf("%d \n",end); 	
	int destfd=1; 
	int hope = dubPipe(fd,end,destfd);
	printf("%d %d %d \n", hope, end, destfd); 	
	return 0; 
}
