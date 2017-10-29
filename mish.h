#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include "sighant.h"
#include "parser.h"

#define WRITE_END  1
#define READ_END 0
extern pid_t childID[MAXCOMMANDS];

/**
 * Name setUpSignalHandeling
 * Descriprion A signalhandeler if SIGINT is send to the program
 */
void setUpSignalHandeling();

/**
 * Name: change cwd
 * Description: Changing the working directory. If the user write .. the
 * function exits the current directory and if another string is used
 * function tries to enter new directory if fail an error is printed.
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv);


/**
 * Name getExecParam
 * Description constructs the parameterList to be used when starting a new
 * process with execvp
 * @param myCommand
 * @return parameterList (matrix of parameters)
 */
char **getExecParam(command myCommand);

/**
 * Name checkFork
 * Description controls if a fork command is executed correct.
 * @param fork
 */
void checkSystemCalls(int fork, char *systemcall);

/**
 * Name redirectInFile
 * Description: Function to redirect input to a file
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectInFile(int fd[], char *infile);

/**
 * Name redirectOutFile
 * Description: Function to redirect output to a file. If the outfile exists
 * the redirect fails.
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectOutFile(int fd[], char *outfile);

/**
 * Name createChildWrite
 * Description creates a child that can only write to the file descriptor
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createChildWrite( int fd[], char **parameterList, command myCommand);

/**
 * Name createChildRead
 * Description Creates a child that can only read from the filedescriptor
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createChildRead(int tempfd[], int fd[], char **parameterList, command myCommand);

/**
 * Name createChildWithoutPipe
 * Description used when only one external command is supposed to be executed
 * here no pipes are created.
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createChildWithoutPipe(int fd[2],char **parameterList, command myCommand);

/**
 * Name createReadAndWriteChild
 * Description creates a child that can pipe to both read and write of the
 * filedescriptor.
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createReadAndWriteChild(int tempfd[],int fd[],char **parameterList, command myCommand);

/**
 * Name waitForChild
 * Description waits for all the childs created during the execprocess
 */
void waitForChild();
