/**
 * Name:signalCommand
 * Description: If crt+c is pressed all children processes are killed
 * @param signalCommand the SIGINT id
 * */
#include <signal.h>
typedef	void	Sigfunc(int);
Sigfunc *mysignal(int signo, Sigfunc *func);

