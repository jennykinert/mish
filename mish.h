#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include "sighant.h"
#include "parser.h"
#include "testProgram.h"

#define WRITE_END  1
#define READ_END 0
#define MAX_COMMAND 5

/**
 * Name: change cwd
 * Description: Changing the working directory. If the user write .. the
 * function exits the current directory and if another string is used
 * function tries to enter new directory if fail an error is printed.
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv);

/**
 * Name: getPath
 * Description: Gets the path of the external program to be executed
 * @param myCommand
 * @return (pointer to path)
 */
char *getPath(command myCommand);

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
void checkFork(int fork);

/**
 * Name redirectInFile
 * Description: Function to redirect input to a file
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectInFile(int fd[], char *infile);
/**
 * Name redirectOutFile
 * Description: Function to redirect stdin or out to a file
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectOutFile(int fd[], char *outfile);

/**
 * Name createChildWrite
 * Description creates a child that can only write to the file descriptor
 * @param fd (file descriptor)
 * @param parameterList
 */
void createChildWrite( int fd[], char **parameterList, command myCommand);
/**
 * Name createChildRead
 * Description Creates a child that can only read from the filedescriptor
 * @param fd (file descriptor)
 * @param parameterList
 */
void createChildRead(int fd[], char **parameterList, command myCommand);

/**
 * Name createChildWithoutPipe
 * Description used when only one external command is supposed to be executed
 * here no pipes are created.
 * @param parameterList
 */
void createChildWithoutPipe(char **parameterList);
/**
 * Name createReadAndWriteChild
 * Description creates a child that can pipe to both read and write of the
 * filedescriptor
 * @param fd
 * @param parameterList
 */
void createReadAndWriteChild(int fd[], char **parameterList);

/**
 * Name waitForChild
 * Description waits for all the childs created during the execprocess
 */
void waitForChild();