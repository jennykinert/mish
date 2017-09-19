#include <stdio.h>
#include <unistd.h>
#include "execute.h"
//Fundera på vad som hör till vad och duplicera den processen. Kom ihåg att stänga alla pipes efter användning!!. 
int dubPipe(int pip[2], int end, int destfd)
{
	//Creates a pair of file descriptors pointing to pipe inode pip[0]=reading pip[1]=writing
	pipe(pip); 
	if(end == READ_END){
		dup(pip[0]);
		close(pip[1]);
		printf("%d \n",destfd);
		return destfd; 
	}
	else if(end == WRITE_END){
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
	int end = 1;
	printf("%d \n",end); 	
	int destfd=0; 
	int hope = dubPipe(fd,end,destfd);
	printf("%d %d %d \n", hope, end, destfd); 	
	return 0; 
}
